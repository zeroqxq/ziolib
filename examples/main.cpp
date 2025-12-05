#include "ziolib.hpp"

int main() {
    zio::Screen screen;

    zio::Window win = zio::Window::centered(5, 26);
    win.box();

    win.title("Welcome to ziolib");
    win.print(CENTER, CENTER, "Hello world!");

    win.refresh();
    wgetch(win);
}