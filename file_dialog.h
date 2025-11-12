#pragma once
#include <string>
#include <optional>


namespace imgpicker {
// Otwórz nieblokujący dialog wyboru obrazu (PNG/JPG). Optionalnie wskaż katalog startowy.
void Open(const std::string& startDir = "");


// Narysuj okno dialogowe (wywołuj co klatkę po ImGui::NewFrame()).
// Zwraca true tylko w tej klatce, w której użytkownik potwierdził wybór.
bool Draw();


// Odbierz wynik (jednorazowo). Jeśli jest dostępny, zwróci ścieżkę pliku; w przeciwnym razie std::nullopt.
std::optional<std::string> TakeResult();


// Czy okno jest aktualnie otwarte?
bool IsOpen();
}