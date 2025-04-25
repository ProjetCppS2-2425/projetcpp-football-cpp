import speech_recognition as sr

recognizer = sr.Recognizer()

with sr.Microphone() as source:
    print("Parlez...")
    audio = recognizer.listen(source)

try:
    print(recognizer.recognize_google(audio, language="fr-FR"))
except sr.UnknownValueError:
    print("Je n'ai pas compris.")
except sr.RequestError as e:
    print(f"Erreur de service : {e}")
