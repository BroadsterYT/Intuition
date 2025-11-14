cd %~dp0
python -m venv .venv
call .venv\Scripts\activate.bat
pip install -r requirements.txt
pyinstaller --onefile --collect-all requests intumi.py