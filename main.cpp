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
        x += stepX; if(x > 800) x -= 800; if(x < 0) x += 800;
        y += stepY; if(y > 600) y -= 600; if(y < 0) y += 600;
        cout << x << " " << y << endl;
    }
    moveLeft()
    {
        if(stepX == 10 && stepY == 0){}
        else{
            stepX = -10;
            stepY = 0;
        }
    }
    moveRight()
    {
        if(stepX == -10 && stepY == 0){}
        else{
            stepX = 10;
            stepY = 0;
        }
    }
    moveDown()
    {
        if(stepX == 0 && stepY == -10){}
        else{
            stepY = 10;
            stepX = 0;
        }
    }
    moveUp()
    {
        if(stepX == 0 && stepY == 10){}
        else{
            stepY = -10;
            stepX = 0;
        }
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

    int R;
    int G;
    int B;

    Body(int _x, int _y, Body* _next, int _R, int _G, int _B)
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
        SDL_SetRenderDrawColor(renderer, 255, R, G, B);
        SDL_RenderFillRect(renderer, &body);
    }

    void addLast(int _x, int _y, int _R, int _G, int _B)
    {
        Body* newBody = new Body(_x, _y, NULL, _R, _G, _B);
        Body *p = head;
        while(p->next != NULL){
            p = p -> next;
        }
        p -> next = newBody;
        newBody -> prev = p;

   /*     R = _R;
        G = _G;
        B = _B;*/
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
    int hardLevel = 50;   //Độ khó
    int i = 0;
    srand(time(0));
    int length = 1;   // Độ dài thân rắn
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE); //Cửa sổ

    Lead lead;  // Tạo đầu rắn
    SDL_Event e;
    bool quit = false;

    Body *head = new Body(lead.x - 10, lead.y, NULL, 0, 255, 255);            //Tạo thân rắn (khúc đầu tiền)
    head->head = head;                                           //
    Body *p = new Body(lead.x - 10, lead.y, NULL, 0, 255, 255);
    head->x = lead.x -10;
    head->y = lead.y;
    Food food(rand() % 70 * 10 + 50, rand() % 50 * 10 + 50);                                                   // Tạo thức ăn


    // Cập nhật tọa độ đầu mới -> Vẽ thân vẽ đầu -> in -> Cập nhật tọa độ thân -> Kiểm tra có ăn được mồi không -> Nhận tín hiệu từ bàn phím -> Vòng lặp mới

    while(quit == false)
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


        int a = rand();           //Tạo số ngẫu nhiên để tạo tọa độ mồi ngẫu nhiên và tạo màu ngẫu nhiên cho phần thân mới
        int b = rand();
        int c = rand();
        if(lead.x == food.x && lead.y == food.y){          //Nếu ăn được mồi
            food.x = 50 + a % 70 * 10;                     //Thì tạo mồi mới
            food.y = 50 + b % 50 * 10;

            length++;                                      //Tăng chiều dài thêm 1
            head->addLast(-1, -1, a % 256, b % 256, c % 256);
            hardLevel -= 5;                              //Tăng độ khó
            if(hardLevel <= 0) hardLevel = 1;             //Đảm bảo độ khó không vi phạm
        }

    //    cout << "+++++++++++";

        if(length >= 3){
            p = head; p = p->next; p = p->next;
            while(p != NULL){
                if(p->x == lead.x && p->y == lead.y){
                    quit = true;
                }
                p = p->next;
            }
        }


        SDL_Delay(hardLevel);
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
