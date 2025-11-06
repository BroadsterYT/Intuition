import requests
import json
import argparse

parser = argparse.ArgumentParser(description="Groq AI Parameters")
parser.add_argument("--api-key", type=str, required=True)
parser.add_argument("--prompt", type=str, required=True)

args = parser.parse_args()

# endpoint
url = "https://api.groq.com/openai/v1/chat/completions"
headers = {
    "Authorization": f"Bearer {args.api_key}",
    "Content-Type": "application/json"
}

data = {
    "messages": [
        {
            "role": "user",
            "content": args.prompt
        }
    ],
    "model": "llama-3.3-70b-versatile",
}

response = requests.post(url, headers=headers, data=json.dumps(data))

if response.status_code == 200:
    returned_data = response.json()
    message = returned_data['choices'][0]['message']['content']
    print(message)
else:
    print(f"Request failed with status {response.status_code}")
    print(response.text)