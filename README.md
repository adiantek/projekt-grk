# projekt-grk
Projekt z grafiki komputerowej

Wersja online: https://adiantek.github.io/projekt-grk/

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
- Przycisk `2` - zmiana modelu oświetlenia na model 
- Przycisk `F` przy włączonym celowniku - łapanie jednego elementu lub łapanie wielu lub stawienie elementu dynamicznego
- Przycisk `R` - zmiana trybu dla klawisza `F`
- Przycisk `G` w trybie stawiania elementów: zmiana modelu do postawienia (kula, sześcian, itd.)
- Przycisk `E` - stawianie elementu statycznego
- Przycisk `Q` - usuwanie elementu (w trybie stawiania elementu)
- Przycisk `C` - otworzenie lub zamknięcie najbliższej skrzynki (max odległość: 8 jednostek)

## Rozwiązywanie problemów

> Nie odnaleziono pliku `ucrtbased.dll`
> - Problem można rozwiązać instalując ze strony Microsoftu: [Windows 10 SDK](https://developer.microsoft.com/pl-pl/windows/downloads/windows-sdk/) lub w programie Visual Studio.

> Nie odnaleziono pliku `MSVC*.dll`
> - Problem można rozwiązać instalując MSVC version 142 (Recommended).

> P. S. Obie biblioteki najłatwiej zainstalować poprzez instalator składników w Visual Studio - Aby do niego wejść należy uruchomić w panelu sterowania narzędzie `Dodaj lub usuń programy`, a następnie przy instalacji programu wybrać opcję `modyfikuj`. W zakładce `Pojedyńcze składniki` wybieramy pozycje do dodania. Po pomyślnej instalacji zalecane jest ponowne uruchomienie komputera.