
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

// 1. Deklaracja typu student jako struktury
struct Student {
    std::string imie;
    std::string nazwisko;
    int rokUrodzenia;
    std::vector<int> oceny;
    double sredniaOcen = 0.0; // Inicjalizacja na 0
};

// 7. Definicja klasy wyjątku dla ocen poza zakresem
class OcenaPozaZakresemException : public std::exception {
public:
    const char* what() const throw() {
        return "Ocena jest poza dozwolonym zakresem (2-5).";
    }
};

// 8. Definicja klasy wyjątku dla niepoprawnych imion i nazwisk
class NiepoprawneDaneException : public std::exception {
public:
    const char* what() const throw() {
        return "Imie i nazwisko powinny zawierac tylko litery.";
    }
};

// 9. rzucenie wyjątku dzielenie przez zero
class DivisionByZeroException : public std::exception {
public:
    const char* what() const throw() {
        return "Division by zero - tablica ocen jest pusta.";
    }
};

// Funkcja generująca losową ocenę
int losowaOcena() {
    return rand() % 4 + 2; // Zakres ocen od 2 do 5
}

// Funkcja sprawdzająca, czy ciąg znaków zawiera tylko litery
bool tylkoLitery(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isalpha(c); });
}

// 2. Kod tworzący tablicę studentów
std::vector<Student> stworzStudentow(int liczbaStudentow) {
    std::vector<Student> studenci;
    for (int i = 0; i < liczbaStudentow; ++i) {
        Student student;
        student.imie = "Imie" + std::string(1, 'A' + i);
        student.nazwisko = "Nazwisko" + std::string(1, 'A' + i);
        student.rokUrodzenia = 1990 + rand() % 10;

        // Sprawdzenie poprawności imienia i nazwiska
        if (!tylkoLitery(student.imie) || !tylkoLitery(student.nazwisko)) {
            throw NiepoprawneDaneException();
        }

        int liczbaOcen = 5 + rand() % 6; // Losowanie liczby ocen
        for (int j = 0; j < liczbaOcen; ++j) {
            int ocena = losowaOcena();
            if (ocena < 2 || ocena > 5) {
                throw OcenaPozaZakresemException();
            }
            student.oceny.push_back(ocena);
        }
        studenci.push_back(student);
    }
    return studenci;
}

// 4. Kod liczący średnią ocen
void obliczSrednieOceny(std::vector<Student>& studenci) {
    for (auto& student : studenci) {
        if (student.oceny.empty()) {
            // 9. rzucenie wyjątku dzielenie przez zero
            throw DivisionByZeroException();
        }
        double suma = 0;
        for (int ocena : student.oceny) {
            suma += ocena;
        }
        student.sredniaOcen = suma / student.oceny.size();
    }
}

//12. Zmodyfikuj funkcję wyświetlającą studentów tak, aby wyświetlała nazwiska wielkimi literami.
std::string naWielkieLitery(const std::string& input) {
    std::string wynik;
    wynik.resize(input.size());
    std::transform(input.begin(), input.end(), wynik.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return wynik;
}

// 3. Kod wyświetlający tablicę studentów
void wyswietlStudentow(const std::vector<Student>& studenci) {
    for (const auto& student : studenci) {
        std::cout << "Imie: " << student.imie
                  << ", Nazwisko: " << naWielkieLitery(student.nazwisko)
                  << ", Rok urodzenia: " << student.rokUrodzenia
                  << ", Oceny: ";
                        for (const auto& ocena : student.oceny) {
                            std::cout << ocena << " ";
                        }
                        std::cout
                  << ", Srednia ocen: " << student.sredniaOcen << std::endl;
    }
}

// 11. Napisz kod umożliwiający wyszukanie studenta po fragmencie nazwiska.
void wyszukajPoFragmencieNazwiska(const std::vector<Student>& studenci, const std::string& fragment) {
    bool znaleziono = false;
    for (const auto& student : studenci) {
        if (student.nazwisko.find(fragment) != std::string::npos) {
            std::cout << "Znaleziono studenta: " << student.imie << " " << student.nazwisko
                      << ", Rok urodzenia: " << student.rokUrodzenia
                      << ", Srednia ocen: " << student.sredniaOcen << std::endl;
            znaleziono = true;
        }
    }
    if (!znaleziono) {
        std::cout << "Nie znaleziono studentow o nazwisku zawierajacym: " << fragment << std::endl;
    }
}



int main() {
    try {
        srand(static_cast<unsigned>(time(nullptr)));
        auto studenci = stworzStudentow(5);
        obliczSrednieOceny(studenci);

        //6. sortowanie studentów od najwyższej do najniższej średniej
        std::sort(studenci.begin(), studenci.end(), [](const Student& a, const Student& b) {
            return a.sredniaOcen > b.sredniaOcen;
        });

        wyswietlStudentow(studenci);

        // 11. Napisz kod umożliwiający wyszukanie studenta po fragmencie nazwiska.
        std::cout << "Wyszukiwanie studentow po fragmencie nazwiska: " << std::endl;
        wyszukajPoFragmencieNazwiska(studenci, "NazwiskoC");


        // Tutaj dodać resztę logiki dla punktów 5, 9-12
    } catch (const std::exception& e) {
        std::cerr << "Wyjatek: " << e.what() << std::endl;
    }
    return 0;
}
