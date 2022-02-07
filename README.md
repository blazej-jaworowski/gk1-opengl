# Grafika Komputerowa 1 | Zadanie 4
## Opis
Projekt skupia się na dzielnej pszczółce która na nasze polecenie lata od kwiatka do kwiatka.
## Użyte technologie
- C++
- OpenGL (GLFW, GLEW, GLM)
## Jak zbudować
```
cmake . && cmake --build .
```
## Klawisze
Zmiana kamery:
- 8 - kamera statyczna
- 9 - kamera obserwująca pszczołę
- 0 - kamera latająca za pszczołą

Zmiana celu pszczółki:
- 1 - pszczoła poleci do czerwonego kwiatka
- 2 - pszczoła poleci do niebieskiego kwiatka
- 3 - pszczoła poleci do żółtego kwiatka

Zmiana cieniowania:
- U - zmiana cieniowania na stałe
- I - zmiana cieniowania na Phonga
- O - zmiana cieniowania na Gourauda

Inne:
- V - włączanie / wyłączanie mgły
- B - zmiana modelu oświetlenia
- N - zmiana dzień / noc

Modele zostały pobrane ze strony https://www.cgtrader.com/.