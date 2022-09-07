#include "ncurses_display.h"
#include "system.h"
#include "format.h"

int main() {
  std::string time  = Format::ElapsedTime(61);
  System system;
  NCursesDisplay::Display(system);

}