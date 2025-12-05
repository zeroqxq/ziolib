#pragma once
#include <ncurses.h>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>

#define LEFT    -1111
#define CENTER  -2222
#define RIGHT   -3333
#define TOP     -1111
#define BOTTOM  -3333

namespace zio {

struct Color {
    static constexpr short Default   = -1;
    static constexpr short Black     = COLOR_BLACK;
    static constexpr short Red       = COLOR_RED;
    static constexpr short Green     = COLOR_GREEN;
    static constexpr short Yellow    = COLOR_YELLOW;
    static constexpr short Blue      = COLOR_BLUE;
    static constexpr short Magenta   = COLOR_MAGENTA;
    static constexpr short Cyan      = COLOR_CYAN;
    static constexpr short White     = COLOR_WHITE;
};


inline void setlocale(const char* loc = "") noexcept {
    ::setlocale(LC_ALL, loc);
}

class Screen {
public:
    Screen()  { 
        initscr(); 
        cbreak(); 
        noecho(); 
        keypad(stdscr, TRUE); 
        curs_set(0);
        if (has_colors()) { 
            start_color(); 
            use_default_colors(); 
        } 
    }

    ~Screen() {
         endwin(); 
    }

    static int height() { 
        return LINES; 
    }

    static int width()  { 
        return COLS;  
    }
};

class Window {

    protected:

        WINDOW* win;

    public:

        enum class TitleAlign { 
            Left, 
            Center, 
            Right 
        };


        Window(int h, int w, int y, int x) : win(newwin(h, w, y, x)) { 
            keypad(win, TRUE); 
        }

        static Window centered(int h, int w) {
            return Window(h, w,
                        (Screen::height() - h) / 2,
                        (Screen::width()  - w) / 2);
        }

        static Window centered_x(int h, int w, int fixed_y) {
            return Window(h, w, fixed_y, (Screen::width() - w) / 2);
        }

        static Window centered_y(int h, int w, int fixed_x) {
            return Window(h, w, (Screen::height() - h) / 2, fixed_x);
        }

        
        Window(const Window&) = delete;
        
        Window& operator=(const Window&) = delete;
       
        virtual ~Window() { 
            if (win) {
                delwin(win);
            } 
        }

        operator WINDOW*() const { 
            return win; 
        }
        
        void refresh() { 
            wrefresh(win); 
        }

        void clear() { 
            werase(win); 
        }

        void box(int v = 0, int h = 0) { 
            ::box(win, v, h); 
        }
        
        void print(int y, int x, const std::string& text,
           int attr = A_NORMAL, short pair = 0) const {

            if (!win || text.empty()) return;

            int height = getmaxy(win);
            int width  = getmaxx(win);
            int len    = static_cast<int>(text.length());

            int real_y = y;
            if (y == TOP) real_y = 1;                                          
            else if (y == CENTER) real_y = (height - 1) / 2;
            else if (y == BOTTOM) real_y = height - 2;

            int real_x = x;
            if (x == LEFT) real_x = 2;
            else if (x == CENTER) {
                int inner_w = width - 4; 
                real_x = 2 + (inner_w - len) / 2;
            }
            else if (x == RIGHT) real_x = width - len - 2;

   
            real_y = std::max(1, std::min(real_y, height - 2));
            real_x = std::max(1, std::min(real_x, width  - len - 1));

            
            attr_t old_attr; short old_pair;
            wattr_get(win, &old_attr, &old_pair, nullptr);
            wattr_set(win, attr | COLOR_PAIR(pair), pair, nullptr);
            mvwprintw(win, real_y, real_x, "%s", text.c_str());
            wattr_set(win, old_attr, old_pair, nullptr);
        }

    void title(const std::string& text,
           TitleAlign align = TitleAlign::Center,
           int attr = A_BOLD,
           short pair = 0) const {
        if (text.empty() || !win) return;

        int width = getmaxx(win);
        int len   = static_cast<int>(text.length());

        int x;
        switch (align) {
            case TitleAlign::Left: x = 2; break;
            case TitleAlign::Right: x = width - len - 2; break;
            case TitleAlign::Center:
            default:
                x = (width - len) / 2;
                break;
        }

        attr_t old_attr; short old_pair;
        wattr_get(win, &old_attr, &old_pair, nullptr);
        wattr_set(win, attr | COLOR_PAIR(pair), pair, nullptr);

        mvwprintw(win, 0, x, "%s", (" " + text + " ").c_str());

        wattr_set(win, old_attr, old_pair, nullptr);
    }
};
}