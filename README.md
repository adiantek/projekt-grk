# projekt-grk
Projekt z grafiki komputerowej

Wersja online: https://adiantek.github.io/projekt-grk/ \
Wersja do pobrania: https://github.com/adiantek/projekt-grk/releases/download/1.0/GrafikaKomputerowa.zip \
Wymagania projektu ze specyfikacji: https://github.com/adiantek/projekt-grk/blob/main/projekt.md \
Podsumowanie projektu: https://github.com/adiantek/projekt-grk/wiki/Podsumowanie-projektu

Wymagane:
- [CMake](https://cmake.org/download/)

Opcjonalne (zalecane):
- [Visual Studio Code](https://code.visualstudio.com/)
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

Budowanie dla osób mających życie:
- Przycisk `F7` aby zbudować projekt (w vscode)
- Przycisk `F5` aby uruchomić projekt (z debugowaniem)

Budowanie dla osób nie mających życia:
- `mkdir build`
- `cd build`
- `cmake ..`
- `cmake --build .`

## Sterowanie
- Przycisk `1` - zmiana oświetlenia na PBR
- Przycisk `2` - zmiana modelu oświetlenia na model Phonga
- Przycisk `F` przy włączonym celowniku - łapanie jednego elementu lub łapanie wielu lub stawienie elementu dynamicznego
- Przycisk `R` - zmiana trybu dla klawisza `F`
- Przycisk `G` w trybie stawiania elementów: zmiana modelu do postawienia (kula, sześcian, itd.)
- Przycisk `E` - stawianie elementu statycznego
- Przycisk `Q` - usuwanie elementu (w trybie stawiania elementu)
- Przycisk `C` - otworzenie lub zamknięcie najbliższej skrzynki (max odległość: 8 jednostek)
- Przyciski `W`/`A`/`S`/`D` - przemieszczanie się robotem
- Przycisk `LEWY SHIFT` - szybciej porusza się robot
- Przycisk `SPACJA` - w górę porusza się robot
- Przycisk `LEWY ALT` - w dół porusza się robot

## Tryby celownika
- Plus - kamera wchodzi w interakcje z jednym dokładnie wskazanym obiektem,
- Krzyżyk - kamera wchodzi w interakcję z grupą obiektów wskazywaną wokół kursora,
- Budowanie - budowanie elementów w miejscu wskazanym przez kamerę.

## Wykorzystane biblioteki
- https://github.com/madler/zlib
- https://github.com/glennrp/libpng
- https://github.com/assimp/assimp
- https://github.com/glfw/glfw
- https://github.com/NVIDIAGameWorks/PhysX
- https://github.com/kcat/openal-soft
- https://emscripten.org/ (wersja przeglądarkowa tylko)

## Rozwiązywanie problemów

> Nie odnaleziono pliku `ucrtbased.dll`
> - Problem można rozwiązać instalując ze strony Microsoftu: [Windows 10 SDK](https://developer.microsoft.com/pl-pl/windows/downloads/windows-sdk/) lub w programie Visual Studio.

> Nie odnaleziono pliku `MSVC*.dll`
> - Problem można rozwiązać instalując MSVC version 142 (Recommended).

> P. S. Obie biblioteki najłatwiej zainstalować poprzez instalator składników w Visual Studio - Aby do niego wejść należy uruchomić w panelu sterowania narzędzie `Dodaj lub usuń programy`, a następnie przy instalacji programu wybrać opcję `modyfikuj`. W zakładce `Pojedyńcze składniki` wybieramy pozycje do dodania. Po pomyślnej instalacji zalecane jest ponowne uruchomienie komputera.
