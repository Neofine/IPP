/**
 * @file
 * Moduł przetrzymujący funkcje pomocnicze do modułu interactive.h
 *
 * @author Jakub Panasiuk
 */
#ifndef ASSIST_INTERACTIVE_H
#define ASSIST_INTERACTIVE_H

#include <inttypes.h>
#include <termios.h>
#include "gamma.h"

/**
 * @brief Enumerator który pozwala w ładny sposób przekazywać errory
 */
enum blunder{already_owned = -7, other_players, too_much_areas, golden_himself,
        free_field, violate_rules, out_of_bounds};

/**
 * @brief Dla danego argumentu zwraca o ile w osi x przesunie się podświetlenie
 * Wywoływany z poziomu z którego wiemy, że 2 poprzednie argumenty wskazują na
 * wciśnięcie strzałki, więc ta funkcja rozszyfrowuje jedynie czy w osi x
 * zmieni się ono jakkolwiek
 * @param[in] c         – zmienna odpowiadająca ostatniemu wczytanemu argumentowi
 * @return wartość o ile i w jaką stronę zmieni się teraz przesunięcie na osi x
 */
int move_horizontal(int c);

/**
 * @brief Dla danego argumentu zwraca o ile w osi y przesunie się podświetlenie
 * Wywoływany z poziomu z którego wiemy, że 2 poprzednie argumenty wskazują na
 * wciśnięcie strzałki, więc ta funkcja rozszyfrowuje jedynie czy w osi y
 * zmieni się ono jakkolwiek
 * @param[in] c          – zmienna odpowiadająca ostatniemu wczytanemu argumentowi
 * @return wartość o ile i w jaką stronę zmieni się teraz przesunięcie na osi y
 */
int move_vertical(int c);

/**
 * @brief Dla danego enuma errora wypisuje odpowiedni do niego tekst
 * Funkcja pomocnicza do @ref move_error i @ref golden_error, obie te
 * funkcje wywołują tę funkcje by wypisała odpowiedni komunikat, wypisuje
 * go pod informacją o graczu, na kolor czerwony
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] error      – enumerator, przekazuje informacje o errorze który
 *                         ta funkcja ma teraz wypisać
 */
void print_error(gamma_t *g, int error);

/**
 * @brief Wypisuje powód dla którego gracz nie może wykonać @ref gamma_move
 * Wywoływana gdy gracz wcisnął przycisk odpowiadający wykonaniu funkcji
 * @ref gamma_move na podświetlonym teraz polu, lecz ta akcja się nie powiodła
 * wypisuje jeden z 3 errorów dla których akcja się potencjalnie nie udała
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player     – numer gracza, liczba dodatnia, mniejsza od wartości
 *                         @p players z funkcji @ref gamma_new,
 * @param[in] x          – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] y          – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 */
void move_error(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/**
 * @brief Wypisuje powód dla którego gracz nie może wykonać @ref gamma_golden_move
 * Wywoływana gdy gracz wcisnął przycisk odpowiadający wykonaniu funkcji
 * @ref gamma_golden_move na podświetlonym teraz polu, lecz ta akcja się nie powiodła
 * wypisuje jeden z 3 errorów dla których akcja się potencjalnie nie udała
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player     – numer gracza, liczba dodatnia, mniejsza od wartości
 *                         @p players z funkcji @ref gamma_new,
 * @param[in] x          – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] y          – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 */
void golden_error(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/**
 * @brief sprawdza czy gracz chce zakończyć turę
 * Dla tablicy 3 intów sprawdza czy litera "C" została wciśnięta
 * w ramach strzałki czy gracz na prawdę chce zakończyć turę
 * @param[in] chars    – tablica trzech ostatnich wczytanych znaków
 */
bool skip_turn(int *chars);

/**
 * @brief Zmienia kolor tła i czcionki pola (@p x, @p y)
 * Przestawia kursor w terminalu na odpowiednią pozycję i nadpisuje będący
 * tam tekst, tym ze zmienionym kolorem tła i czcionki,
 * wywołuje również @ref update_cords żeby zmienić wypisywane obecnie
 * koordynaty nad planszą
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x          – numer kolumny, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] y          – numer wiersza, liczba dodatnia mniejsza równa wartości
 *                         @p width z funkcji @ref gamma_new,
 * @param[in] color      – kolor jakim jest wypisywana czcionka na polu, jeżeli
 *                         @p color jest równy "" to kolor jest standardowym
 *                         kolorem w terminalu
 * @param[in] background – kolor jakim jest wypisywane tło na polu, jeżeli
 *                         @p color jest równy "" to kolor jest standardowym
 *                         kolorem w terminalu
 */
void highlight_field(gamma_t *g, uint32_t x, uint32_t y, char * color, char * background);

/**
 * @brief Wypisuje plansze oraz graczy i ich liczbę zajętych pól
 * Wywołana pod koniec trybu interaktywnego, dodatkowo graczy którzy mieli
 * najwięcej zajętych pól wypisuje na zielono, a wszystkich innych na czerwono
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 */
void final_show(gamma_t *g);

/**
 * @brief Wypisuje gracza oraz jego statystyki pod planszą
 * Wypisuje kolejno @p player, @p taken, @p free_fields oraz komunikat
 * czy gracz posiada dostępny złoty ruch, w liniach od @p print_loc, aż do
 * @p print_loc + 3, będących pod planszą
 * @param[in] g               – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player          – numer gracza, liczba dodatnia, mniejsza od wartości
 *                              @p players z funkcji @ref gamma_new
 * @param[in] free_fields     – liczba pól na których gracz @p player może zrobić
 *                              ruch funkcją @ref gamma_move
 * @param[in] golden_possible – wynik funkcji @ref gamma_golden_possible oznacza czy
 *                              gracz może wykonać złoty ruch na planszy
 */
void print_player(gamma_t *g, uint32_t player, uint64_t free_fields, bool golden_possible);

/**
 * @brief Zmienia kolor wyświetlanych pionków gracza @p landlord
 * Jeżeli @p like_rest jest ustawiony na @p false to zmienia kolor pionków
 * gracza na cyjanowy, jeżeli na @p true to zmienia na kolor biały, czyli
 * domyślny kolor pionków
 * @param[in] g          – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] landlord   – numer gracza, liczba dodatnia, mniejsza od wartości
 *                         @p players z funkcji @ref gamma_new,
 * @param[in] like_rest  – bool oznaczający czy wyróżniam pola gracza, lub czy
 *                         przywracam je do stanu pierwotnego
 */
void highlight_owned(gamma_t *g, uint32_t landlord, bool like_rest);

#endif //ASSIST_INTERACTIVE_H
