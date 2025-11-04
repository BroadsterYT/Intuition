/*
  ==============================================================================

    GeminiAPIClient.cpp
    Created: 2025
    Author: Intuition + AI

  ==============================================================================
*/

#include "GeminiAPIClient.h"

GeminiAPIClient::GeminiAPIClient() : juce::Thread("GeminiAPIThread")
{
}

GeminiAPIClient::~GeminiAPIClient()
{
    cancelRequest();
    stopThread(5000);
}

void GeminiAPIClient::sendPrompt(
    const juce::String& apiKey,
    const juce::String& systemPrompt,
    const juce::String& userPrompt,
    SuccessCallback onSuccess,
    ErrorCallback onError
)
{
    if (requestInProgress)
    {
        if (onError)
            onError("A request is already in progress");
        return;
    }

    if (apiKey.isEmpty())
    {
        if (onError)
            onError("API key is empty. Please configure your Google AI Studio API key.");
        return;
    }

    // Store request data
    currentApiKey = apiKey;
    currentSystemPrompt = systemPrompt;
    currentUserPrompt = userPrompt;
    currentSuccessCallback = onSuccess;
    currentErrorCallback = onError;

    requestInProgress = true;
    shouldCancel = false;

    // Start the background thread
    startThread();
}

void GeminiAPIClient::cancelRequest()
{
    shouldCancel = true;
    requestInProgress = false;
}

void GeminiAPIClient::run()
{
    // Build the API endpoint
    juce::String endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + currentApiKey;

    // Build the JSON request body
    juce::DynamicObject::Ptr requestBody = new juce::DynamicObject();

    // Add system instruction
    juce::DynamicObject::Ptr systemInstruction = new juce::DynamicObject();
    juce::DynamicObject::Ptr systemParts = new juce::DynamicObject();
    systemParts->setProperty("text", currentSystemPrompt);
    juce::Array<juce::var> systemPartsArray;
    systemPartsArray.add(juce::var(systemParts.get()));
    systemInstruction->setProperty("parts", systemPartsArray);
    requestBody->setProperty("system_instruction", juce::var(systemInstruction.get()));

    // Add user content
    juce::DynamicObject::Ptr content = new juce::DynamicObject();
    juce::DynamicObject::Ptr parts = new juce::DynamicObject();
    parts->setProperty("text", currentUserPrompt);
    juce::Array<juce::var> partsArray;
    partsArray.add(juce::var(parts.get()));
    content->setProperty("parts", partsArray);
    juce::Array<juce::var> contentsArray;
    contentsArray.add(juce::var(content.get()));
    requestBody->setProperty("contents", contentsArray);

    // Add generation config for JSON output
    juce::DynamicObject::Ptr generationConfig = new juce::DynamicObject();
    generationConfig->setProperty("response_mime_type", "application/json");
    requestBody->setProperty("generationConfig", juce::var(generationConfig.get()));

    juce::String jsonRequest = juce::JSON::toString(juce::var(requestBody.get()));

    if (shouldCancel)
    {
        requestInProgress = false;
        return;
    }

    // Make the HTTP POST request
    juce::URL url(endpoint);
    juce::String responseBody;
    int statusCode = 0;

    try
    {
        juce::URL::InputStreamOptions options = juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
            .withConnectionTimeoutMs(30000)
            .withExtraHeaders("Content-Type: application/json");

        std::unique_ptr<juce::InputStream> stream(url.createInputStream(
            options.withHttpRequestCmd("POST")
                   .withPostData(jsonRequest)
        ));

        if (stream != nullptr)
        {
            statusCode = stream->getStatusCode();
            responseBody = stream->readEntireStreamAsString();
        }
        else
        {
            juce::MessageManager::callAsync([this]() {
                juce::ScopedLock lock(callbackLock);
                if (currentErrorCallback)
                    currentErrorCallback("Failed to connect to Gemini API. Check your internet connection.");
                requestInProgress = false;
            });
            return;
        }
    }
    catch (const std::exception& e)
    {
        juce::MessageManager::callAsync([this, e]() {
            juce::ScopedLock lock(callbackLock);
            if (currentErrorCallback)
                currentErrorCallback("Network error: " + juce::String(e.what()));
            requestInProgress = false;
        });
        return;
    }

    if (shouldCancel)
    {
        requestInProgress = false;
        return;
    }

    // Parse the response
    juce::var parsedResponse = juce::JSON::parse(responseBody);

    if (statusCode == 200 && parsedResponse.isObject())
    {
        // Extract the generated text from response
        auto* responseObj = parsedResponse.getDynamicObject();
        if (responseObj && responseObj->hasProperty("candidates"))
        {
            auto candidates = responseObj->getProperty("candidates");
            if (candidates.isArray() && candidates.size() > 0)
            {
                auto firstCandidate = candidates[0];
                if (firstCandidate.isObject())
                {
                    auto* candidateObj = firstCandidate.getDynamicObject();
                    if (candidateObj && candidateObj->hasProperty("content"))
                    {
                        auto content = candidateObj->getProperty("content");
                        if (content.isObject())
                        {
                            auto* contentObj = content.getDynamicObject();
                            if (contentObj && contentObj->hasProperty("parts"))
                            {
                                auto parts = contentObj->getProperty("parts");
                                if (parts.isArray() && parts.size() > 0)
                                {
                                    auto firstPart = parts[0];
                                    if (firstPart.isObject())
                                    {
                                        auto* partObj = firstPart.getDynamicObject();
                                        if (partObj && partObj->hasProperty("text"))
                                        {
                                            juce::String aiResponse = partObj->getProperty("text").toString();

                                            // Parse the AI response as JSON
                                            juce::var aiResponseJson = juce::JSON::parse(aiResponse);

                                            if (aiResponseJson.isObject())
                                            {
                                                juce::MessageManager::callAsync([this, aiResponseJson]() {
                                                    juce::ScopedLock lock(callbackLock);
                                                    if (currentSuccessCallback)
                                                        currentSuccessCallback(aiResponseJson);
                                                    requestInProgress = false;
                                                });
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // If we got here, the response structure was unexpected
        juce::MessageManager::callAsync([this, responseBody]() {
            juce::ScopedLock lock(callbackLock);
            if (currentErrorCallback)
                currentErrorCallback("Unexpected API response format: " + responseBody.substring(0, 200));
            requestInProgress = false;
        });
    }
    else
    {
        // Handle error response
        juce::String errorMsg = "API request failed with status " + juce::String(statusCode);
        if (parsedResponse.isObject())
        {
            auto* errorObj = parsedResponse.getDynamicObject();
            if (errorObj && errorObj->hasProperty("error"))
            {
                auto error = errorObj->getProperty("error");
                if (error.isObject())
                {
                    auto* errorDetails = error.getDynamicObject();
                    if (errorDetails && errorDetails->hasProperty("message"))
                    {
                        errorMsg = errorDetails->getProperty("message").toString();
                    }
                }
            }
        }

        juce::MessageManager::callAsync([this, errorMsg]() {
            juce::ScopedLock lock(callbackLock);
            if (currentErrorCallback)
                currentErrorCallback(errorMsg);
            requestInProgress = false;
        });
    }
}
