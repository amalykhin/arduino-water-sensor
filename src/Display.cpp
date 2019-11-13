#include <Wire.h>
#include <Adafruit_SSD1306.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#define OLED_ADDR 0x3C

typedef Adafruit_SSD1306 Adafruit; 

/*
String display_screens[3] = {"Litre/Min", "M3/Min", "Gallon/Min"};
int display_currentScreen = 0;
Adafruit* display_init() {
    Adafruit display(-1);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(27,30);
    display.print("Ready");
    return &display;
}
void display_showMenu(Adafruit* display) {
    display->clearDisplay();
    for (int i = 0; i < 3; i++) {
        display->setCursor(27,30+5*i);
        if (i == display_currentScreen) {
            display->print(">" + display_screens[display_currentScreen]);
        } else {
            display->print(" " + display_screens[display_currentScreen]);
        }
    }
}
*/

class Display {
    Adafruit_SSD1306 display;
    String screens[3] = {"l/min", "m3/min", "gal/min"};
    int currentScreen = 0;

public:
    Display() : display(-1){
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(27,30);
        display.print("Ready");
        display.display();
    }

    void nextScreen() {
        currentScreen = (currentScreen + 1) % 3;
        showMenu();
    }

    void showMenu() {
        display.clearDisplay();
        for (int i = 0; i < 3; i++) {
            display.setCursor(27,20+10*i);
            if (i == currentScreen) {
                display.print("> " + screens[currentScreen]);
            } else {
                display.print("  " + screens[i]);
            }
        }
        display.display();
    }

    void showValue(float value) {
        display.clearDisplay();

        display.setCursor(17,30);
        display.print("Debit:");
        display.setCursor(57, 30);
        display.print(value);
        display.setCursor(87, 30);
        display.print(screens[currentScreen]);

        display.display();
    }
};
#endif