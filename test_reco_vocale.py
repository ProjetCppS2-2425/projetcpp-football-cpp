import speech_recognition as sr

recognizer = sr.Recognizer()

with sr.Microphone() as source:
    print("Écoute en cours...")
    audio = recognizer.listen(source)

try:
    print("Vous avez dit : " + recognizer.recognize_google(audio, language="fr-FR"))
except sr.UnknownValueError:
    print("Je n'ai pas compris.")
except sr.RequestError as e:
    print(f"Erreur de service : {e}")
