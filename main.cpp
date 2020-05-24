#include <iostream>
#include <SDL.h>
#include "Function.h"
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Age Of LTNC";
struct Lead
{
    int x = 10;
    int y = 10;  //tọa độ ban đầu
    int stepX = 0; //độ dài bước nhảy mỗi vòng lặp
    int stepY = 0;
    int size = 10;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect lead;
        lead.x = x;
        lead.y = y;
        lead.w = size;
        lead.h = size;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
        SDL_RenderFillRect(renderer, &lead);
    }

    void move()
    {
        x += stepX;
        y += stepY;
        cout << x << " " << y << endl;
    }

    moveLeft()
    {
        stepX = -10;
        stepY = 0;
    }

    moveRight()
    {
        stepX = 10;
        stepY = 0;
    }

    moveDown()
    {
        stepY = 10;
        stepX = 0;
    }

    moveUp()
    {
        stepY = -10;
        stepX = 0;
    }

    bool inside(int minX, int minY, int maxX, int maxY)
    {
        return (minX < x && minY < y && x + size <= maxX && y + size <= maxY);
    }
};

struct Body
{
    int x;
    int y;
    Body* next;
    Body* head;
    Body* prev;
    Body(int _x, int _y, Body* _next)
    {
        x = _x;
        y = _y;
        next = _next;
        head = NULL;
    }

    void render(SDL_Renderer* renderer)
    {
        SDL_Rect body;
        body.x = x;
        body.y = y;
        body.w = 10;
        body.h = 10;
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(renderer, &body);
    }

    void addLast(int _x, int _y)
    {
        Body* newBody = new Body(_x, _y, NULL);
        Body *p = head;
        while(p->next != NULL){
            p = p -> next;
        }
        p -> next = newBody;
        newBody -> prev = p;
    }
};

struct Food{
    int x;
    int y;

    Food(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect body;
        body.x = x;
        body.y = y;
        body.w = 10;
        body.h = 10;
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
        SDL_RenderFillRect(renderer, &body);
    }
};



int main(int argc, char* argv[])
{
    int i = 0;
    srand(time(0));
    int length = 1;   // Độ dài thân rắn
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE); //Cửa sổ

    Lead lead;  // Tạo đầu rắn
    SDL_Event e;
    bool quit = false;

    Body *head = new Body(lead.x - 10, lead.y, NULL);            //Tạo thân rắn (khúc đầu tiền)
    head->head = head;                                           //
    Body *p = new Body(lead.x - 10, lead.y, NULL);
    head->x = lead.x -10;
    head->y = lead.y;
    Food food(300, 300);                                                   // Tạo thức ăn
//    head->addLast(100, 100);head->addLast(100, 100);head->addLast(100, 100);

    // Cập nhật tọa độ đầu mới -> Vẽ thân vẽ đầu -> in -> Cập nhật tọa độ thân -> Kiểm tra có ăn được mồi không -> Nhận tín hiệu từ bàn phím -> Vòng lặp mới

    while(true)  //box.inside(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
    {
        lead.move();                 //Cập nhật tọa độ đầu rắn


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền
        SDL_RenderClear(renderer);                                             //Xóa màn hình cũ

        food.render(renderer);

        p = head;                                                                                        //Con trỏ nháp p kiểu Body
        p->render(renderer);                             // Vẽ khúc thân đầu tiên
        if(length >= 2){
            do{
                p = p->next;                                                            //Vẽ khúc thân thứ 2 trở đi
                p->render(renderer);
            }
            while(p->next != NULL);
        }

        lead.render(renderer);             //Vẽ đầu rắn
        SDL_RenderPresent(renderer);                                          //Cập nhật màn hình( đẩy những thứ hiện tại lên màn hình)


        head->x = lead.x;
        head->y = lead.y;                            //Cập nhật tọa độ thân rắn khúc 1

        if(length >= 2){                //Cập nhật tọa độ thân rắn từ khúc 2 trở đi
            do{
                p->x = p->prev->x;
                p->y = p->prev->y;
                p = p->prev;
            }
            while(p != head);
        }


        int a = rand();           //Tạo số ngẫu nhiên để tạo tọa độ mồi ngẫu nhiên
        int b = rand();
        if(lead.x == food.x && lead.y == food.y){          //Nếu ăn được mồi
            food.x = 50 + a % 70 * 10;                     //Thì tạo mồi mới
            food.y = 50 + b % 50 * 10;

            length++;                                      //Tăng chiều dài thêm 1
            head->addLast(-1, -1);
        }

        SDL_Delay(30);
        if ( SDL_PollEvent(&e) == 0) continue;
        // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;

        if (e.type == SDL_KEYDOWN) {
        	switch (e.key.keysym.sym) {
        		case SDLK_ESCAPE:break;

        		case SDLK_LEFT: lead.moveLeft(); break;
            	case SDLK_RIGHT: lead.moveRight(); break;
            	case SDLK_DOWN: lead.moveDown(); break;
            	case SDLK_UP: lead.moveUp(); break;

        		default: break;
			}
        }

    }
    quitSDL(window, renderer);
    return 0;
}
