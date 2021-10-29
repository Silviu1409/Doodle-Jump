#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <sstream>
#include <windows.h>

using namespace sf;

struct point{
    int x, y;
};

void populatePlats(point plat[], int n){    ///pt popularea platformelor pe ecran(jumate din totalul alocat)
    for(int i=0; i<n; i++){
        plat[i].x=rand()%332;
        plat[i].y=rand()%533;
        if (n>5 && i==n-1){
            plat[i].x = 150;
            plat[i].y = 500;
        }
    }
}

void movePlats(point plat[], int n, int direction[], int i, point plat_normal[], point plat_destruct[], point plat_bouncy[], int k, int l, int m){   ///pt miscarea platformelor

    ///sistem de deplasare a platformelor dupa directie
    if (direction[i] == 0) plat[i].x += 1;
    else plat[i].x -= 1;

    ///sistem de detectare bound-uri(stanga-dreapta si schimbarea directiei in sensul opus
    if (plat[i].x > 330) direction[i] = 1;
    else if (plat[i].x < 0) direction[i] = 0;

    int j;

    ///sistem de detectare al altor platforme si schimbarea directiei la contact
    for (j=0; j<k; j++)
        if ( (plat_normal[j].x-72<=plat[i].x && plat[i].x<=plat_normal[j].x+72) && ((plat_normal[j].y-14<=plat[i].y && plat[i].y<=plat_normal[j].y+14)) ){
            if (direction[i] == 0) direction[i] = 1;
            else direction[i] = 0;
            break;
        }
    for (j=0; j<l; j++)
        if ( (plat_destruct[j].x-72<=plat[i].x && plat[i].x<=plat_destruct[j].x+72) && ((plat_destruct[j].y-14<=plat[i].y && plat[i].y<=plat_destruct[j].y+14)) ){
            if (direction[i] == 0) direction[i] = 1;
            else direction[i] = 0;
            break;
        }
    for (j=0; j<m; j++)
        if ( (plat_bouncy[j].x-72<=plat[i].x && plat[i].x<=plat_bouncy[j].x+72) && ((plat_bouncy[j].y-26<=plat[i].y && plat[i].y<=plat_bouncy[j].y+26)) ){
            if (direction[i] == 0) direction[i] = 1;
            else direction[i] = 0;
            break;
        }
    for (j=0; j<n; j++)
        if ( (plat[j].x-72<=plat[i].x && plat[i].x<=plat[j].x+72) && ((plat[j].y-14<=plat[i].y && plat[i].y<=plat[j].y+14)) ){
            if (direction[i] == 0) direction[i] = 1;
            else direction[i] = 0;
            if (direction[j] == 0) direction[j] = 1;
            else direction[j] = 0;
            break;
        }

}

void drawPlat(RenderWindow &app, int n, Sprite sPlat, point plat[]){    ///da render la platforme pe ecran
    for (int i=0;i<n;i++){
        sPlat.setPosition(plat[i].x,plat[i].y);
        app.draw(sPlat);
    }
}

void generatePlats(point plat[], int n, float dy){    ///pt re-popularea platformelor pe ecran
    for (int i=0;i<n;i++){
        plat[i].y = plat[i].y - dy;
        if (plat[i].y>533) {
            if (n>1){
                if (i<=n/4) plat[i].y = -rand()%41;
                else if (i<=n/2) plat[i].y = -82-rand()%82;
                else if (i<=3*n/4) plat[i].y = -168-rand()%82;
                else plat[i].y = -205-rand()%41;
            }
            else plat[i].y = -82-rand()%123;
            plat[i].x=rand()%332;
        }
    }
}

///sistem de detectare a overlaping-ului intre platforme(daca 2 platforme se afla una peste alta cu minim un pixel, se forteaza mutarea la alta poztitie, pana cand toate blocurile nu sunt overlapped)
void overlaping_detector(point plat_normal[], point plat_destruct[], point plat_floating[], point plat_bouncy[], int k, int l, int m, int n){
    int i, j;
    for(i=0; i<k; i++){
        for(j = i+1; j<k; j++)
            while ( (plat_normal[j].x-68<=plat_normal[i].x && plat_normal[i].x<=plat_normal[j].x+68) && ((plat_normal[j].y-14<=plat_normal[i].y && plat_normal[i].y<=plat_normal[j].y+14)) ){
                plat_normal[i].x=rand()%332;
                plat_normal[i].y=-rand()%76;
            }
        for (j=0; j<l; j++)
            if ( (plat_destruct[j].x-68<=plat_normal[i].x && plat_normal[i].x<=plat_destruct[j].x+68) && ((plat_destruct[j].y-14<=plat_normal[i].y && plat_normal[i].y<=plat_destruct[j].y+14)) ){
                plat_normal[i].x=rand()%332;
                plat_normal[i].y=-rand()%76;
            }
        for (j=0; j<m; j++)
            if ( (plat_floating[j].x-68<=plat_normal[i].x && plat_normal[i].x<=plat_floating[j].x+68) && ((plat_floating[j].y-14<=plat_normal[i].y && plat_normal[i].y<=plat_floating[j].y+14)) ){
                plat_normal[i].x=rand()%332;
                plat_normal[i].y=-rand()%76;
            }
        for (j=0; j<n; j++)
            if ( (plat_bouncy[j].x-68<=plat_normal[i].x && plat_normal[i].x<=plat_bouncy[j].x+68) && ((plat_bouncy[j].y-26<=plat_normal[i].y && plat_normal[i].y<=plat_bouncy[j].y+26)) ){
                plat_normal[i].x=rand()%332;
                plat_normal[i].y=-rand()%76;
            }
    }
    for(i=0; i<l; i++){
        for(j = i+1; j<l; j++)
            while ( (plat_destruct[j].x-68<=plat_destruct[i].x && plat_destruct[i].x<=plat_destruct[j].x+68) && ((plat_destruct[j].y-14<=plat_destruct[i].y && plat_destruct[i].y<=plat_destruct[j].y+14)) ){
                plat_destruct[i].x=rand()%332;
                plat_destruct[i].y=-rand()%76;
            }
        for (j=0; j<k; j++)
            if ( (plat_normal[j].x-68<=plat_destruct[i].x && plat_destruct[i].x<=plat_normal[j].x+68) && ((plat_normal[j].y-14<=plat_destruct[i].y && plat_destruct[i].y<=plat_normal[j].y+14)) ){
                plat_destruct[i].x=rand()%332;
                plat_destruct[i].y=-rand()%76;
            }
        for (j=0; j<m; j++)
            if ( (plat_floating[j].x-68<=plat_destruct[i].x && plat_destruct[i].x<=plat_floating[j].x+68) && ((plat_floating[j].y-14<=plat_destruct[i].y && plat_destruct[i].y<=plat_floating[j].y+14)) ){
                plat_destruct[i].x=rand()%332;
                plat_destruct[i].y=-rand()%76;
            }
        for (j=0; j<n; j++)
            if ( (plat_bouncy[j].x-68<=plat_destruct[i].x && plat_destruct[i].x<=plat_bouncy[j].x+68) && ((plat_bouncy[j].y-26<=plat_destruct[i].y && plat_destruct[i].y<=plat_bouncy[j].y+26)) ){
                plat_destruct[i].x=rand()%332;
                plat_destruct[i].y=-rand()%76;
            }
    }
    for(i=0; i<m; i++){
        for(j = i+1; j<m; j++)
            while ( (plat_floating[j].x-68<=plat_floating[i].x && plat_floating[i].x<=plat_floating[j].x+68) && ((plat_floating[j].y-14<=plat_floating[i].y && plat_floating[i].y<=plat_floating[j].y+14)) ){
                plat_floating[i].x=rand()%332;
                plat_floating[i].y=-rand()%76;
            }
        for (j=0; j<k; j++)
            if ( (plat_normal[j].x-68<=plat_floating[i].x && plat_floating[i].x<=plat_normal[j].x+68) && ((plat_normal[j].y-14<=plat_floating[i].y && plat_floating[i].y<=plat_normal[j].y+14)) ){
                plat_floating[i].x=rand()%332;
                plat_floating[i].y=-rand()%76;
            }
        for (j=0; j<l; j++)
            if ( (plat_destruct[j].x-68<=plat_floating[i].x && plat_floating[i].x<=plat_destruct[j].x+68) && ((plat_destruct[j].y-14<=plat_floating[i].y && plat_floating[i].y<=plat_destruct[j].y+14)) ){
                plat_floating[i].x=rand()%332;
                plat_floating[i].y=-rand()%76;
            }
        for (j=0; j<n; j++)
            if ( (plat_bouncy[j].x-68<=plat_floating[i].x && plat_floating[i].x<=plat_bouncy[j].x+68) && ((plat_bouncy[j].y-26<=plat_floating[i].y && plat_floating[i].y<=plat_bouncy[j].y+26)) ){
                plat_floating[i].x=rand()%332;
                plat_floating[i].y=-rand()%76;
            }
    }
    for(i=0; i<n; i++){
        for (j=i+1; j<n; j++)
            while ( (plat_bouncy[j].x-68<=plat_bouncy[i].x && plat_bouncy[i].x<=plat_bouncy[j].x+68) && ((plat_bouncy[j].y-26<=plat_bouncy[i].y && plat_bouncy[i].y<=plat_bouncy[j].y+26)) ){
                plat_bouncy[i].x=rand()%332;
                plat_bouncy[i].y=-rand()%76;
            }
        for (j=0; j<k; j++)
            if ( (plat_normal[j].x-68<=plat_bouncy[i].x && plat_bouncy[i].x<=plat_normal[j].x+68) && ((plat_normal[j].y-14<=plat_bouncy[i].y && plat_bouncy[i].y<=plat_normal[j].y+14)) ){
                plat_bouncy[i].x=rand()%332;
                plat_bouncy[i].y=-rand()%76;
            }
        for (j=0; j<l; j++)
            if ( (plat_destruct[j].x-68<=plat_bouncy[i].x && plat_bouncy[i].x<=plat_destruct[j].x+68) && ((plat_destruct[j].y-14<=plat_bouncy[i].y && plat_bouncy[i].y<=plat_destruct[j].y+14)) ){
                plat_bouncy[i].x=rand()%332;
                plat_bouncy[i].y=-rand()%76;
            }
        for (j=0; j<m; j++)
            if ( (plat_floating[j].x-68<=plat_bouncy[i].x && plat_bouncy[i].x<=plat_floating[j].x+68) && ((plat_floating[j].y-14<=plat_bouncy[i].y && plat_bouncy[i].y<=plat_floating[j].y+14)) ){
                plat_bouncy[i].x=rand()%332;
                plat_bouncy[i].y=-rand()%76;
            }
    }
}

///functie pt a seta fontul, textul, marimea, coordonatele pt un text
void setcaract_text(Text &text, const Font& font, const std::ostringstream& ss, const int font_size, const float x, const float y){
    text.setFont(font);
    text.setString(ss.str());    ///ss.str() transforma tot ce contine ss in string
    text.setCharacterSize(font_size);
    text.setPosition(x, y);
}

int highScore=0, score=0;   ///retin scorul maxim si cel dupa o sesiune in joc

bool Menu(RenderWindow &app){

    Texture t1,t2,t3;   ///texturile din meniu
    t1.loadFromFile("images/menu_background.png");  ///background
    t2.loadFromFile("images/play.png"); ///buton play
    t3.loadFromFile("images/quit.png"); ///buton quit

    Sprite sBackground(t1), sPlay(t2),  sQuit(t3);  ///sprite-uri pt texturi
    sPlay.setPosition(75, 215); ///coordonate in meniu buton Play
    sQuit.setPosition(250, 200);    ///coordonate in meniu buton Quit

    app.draw(sBackground);  ///deseneaza background-ul
    app.draw(sPlay);    ///deseneaza butonul de play
    app.draw(sQuit);    ///deseneaza butonul de quit
    app.display();  ///afiseaza in aplicatie
    app.setFramerateLimit(0);

    while(app.isOpen()){    ///cat timp e deschis programul
        Event e;

        ///event pt inchiderea aplicatiei din X
        while (app.pollEvent(e)){
            if (e.type == Event::Closed)
                return false;
        }

        if(Mouse::isButtonPressed(Mouse::Left)){    ///la left click
            Vector2f mouse = app.mapPixelToCoords(Mouse::getPosition(app)); ///coord. mouse
            FloatRect bounds1 = sPlay.getGlobalBounds(), bounds2 = sQuit.getGlobalBounds(); ///coord. bound-uri pt butoanele de play si quit
            if (bounds1.contains(mouse)){   ///daca mouse-ul se afla pe butonul de play la left-click
                return true;
            }
            else if (bounds2.contains(mouse)){  ///daca mouse-ul se afla pe butonul de quit la left-click
                    return false;
                }
        }
        Sleep(1);
    }
    return false; ///daca se inchide aplicatia la meniu, se returneaza false(pt inchidere program)
}

int Game(RenderWindow &app){

    srand(time(0)); ///seeded rand, ca numerele generate random sa nu fie aceleasi la fiecare loop

    app.setFramerateLimit(80);

    Texture t1,t2,t3,t4,t5,t6,t7,t8,t9; ///texturile din joc
    t1.loadFromFile("images/background.png");   ///background
    t2.loadFromFile("images/platform.png"); ///platforma normala
    t3.loadFromFile("images/platform_destruct.png");    ///platforma care se distruge la atingere
    t4.loadFromFile("images/platform_floating.png");    ///platforma miscatoare
    t5.loadFromFile("images/platform_bounce.png");  ///platforma cu arc
    t6.loadFromFile("images/doodle.png");   ///doodle
    t7.loadFromFile("images/top_border.png");   ///bordura albastra
    t8.loadFromFile("images/border_border.png");    ///bordura de jos a bordurii albastre
    t9.loadFromFile("images/pause.png");    ///buton pauza

    Sprite sBackground(t1), sPlat1(t2), sPlat2(t3), sPlat3(t4), sPlat4(t5), sPers(t6), sBorder1(t7), sBorder2(t8), sPauseButton(t9);

    point plat1[14], plat2[6], plat3[4], plat4[2];   ///mereu o sa fie pe ecran 7+3+2+1 = 13 platforme randate.

    ///populam harta cu platforme( 7 normale, 6 care se distrug, 2 floating si 1 bouncy)
    populatePlats(plat1, 7);
    populatePlats(plat2, 3);
    populatePlats(plat3, 2);
    populatePlats(plat4, 1);
    overlaping_detector(plat1, plat2, plat3, plat4, 14, 6, 4, 2);

    int x=160, y=0, h=200, mx = 200;    ///coodornate initiale doodle, care se spawneaza la (x, y)=(160, 0) la h=200px; mx va fi util la calc. sistemului de scor.
    float dy=0; ///coord relative pe axa y, physics related
    score=0;    ///reseteaza scorul la 0
    Text text;  ///textul pt afisat
    Font font;  ///retine fontul local
    int isSpring = 0;   ///verifica daca sari pe spring
    int direction[3] = {0,0,0}; ///0 pt stanga la dreapta, 10 pt dreapta la stanga
    std::ostringstream ss;  ///pt afisare text pe ecran
    if(!font.loadFromFile("res/GreatVibes-Regular.ttf")){   ///verif loading;daca nu se da loading bine se iese
        std::cout<<"Error loading file!"<<std::endl;
        app.close();
        return 0;
    }
    while (app.isOpen()){
        ss<<"Score : "<<score;  ///va contine textul de afisat pe ecran

        ///setez caract. textului pt printat pe ecran: font, continut, size, culoare, stil, coordonate
        setcaract_text(text, font, ss, 40, 5, 0);
        text.setFillColor(Color::Black);
        text.setStyle(Text::Bold);

        sPauseButton.setPosition(355, 5);   ///setez poz in joc a butonului pt pauza

        Event e;
        while (app.pollEvent(e)){   ///daca se apasa x din consola, se inchide app
            if (e.type == Event::Closed)
                return 0;
        }

        if(Mouse::isButtonPressed(Mouse::Left)){    ///la left click pe pause button se pune pauza si se schiba iconl dreapta sus si apare text cu "Paused" in stanga jos

            Vector2f mouse = app.mapPixelToCoords(Mouse::getPosition(app)); ///ia coord mouse

            FloatRect bounds = sPauseButton.getGlobalBounds();  ///va contine boundurile pt butonul de pauza(care vor fi practic un patrat cu height si width)

            ///calibrare bounduri(butonul in sine nu este exact cat size-ul imaginii care face butonul)
            bounds.height -=5;
            bounds.width -=5;
            bounds.left +=5;
            bounds.top +=5;

            if (bounds.contains(mouse)){    ///daca mouse-ul se afla in butonul de pauza
                bool isPaused = true;   ///variabila care spune daca jocul e pe pauza sau nu

                std::ostringstream pauseoutput; ///variabila pt output-ul din starea de pauza
                pauseoutput<<"Paused";  ///continutul output-ului

                Texture t10;
                t10.loadFromFile("images/resume.png");  ///buton resume

                Sprite sResumeButton(t10);  ///sprite cu textura butonului de resume
                sResumeButton.setPosition(355, 5);  ///setare coordonate in joc(va da replace la pause button pt ca acum jocul fiind in pauza nu-i mai poti pune pauza, logic)

                Text text2; ///variabila pt textul care va aparea in pauza

                ///setare caract. text: font, continut, size, culoare, stil, coordonate
                setcaract_text(text2, font, pauseoutput, 50, 25, 450);
                text2.setFillColor(Color::Black);
                text2.setStyle(Text::Italic);

                app.draw(text2);    ///desenez textul pe ecran
                app.draw(sResumeButton);    ///desenez butonul de resume pe ecran
                app.display();  ///afisare elemente si loop

                app.setFramerateLimit(0);

                while (isPaused){   ///cat timp programul este in pauza
                    if(Mouse::isButtonPressed(Mouse::Right)){   ///la click dreapta
                        mouse = app.mapPixelToCoords(Mouse::getPosition(app));  ///coordonate noi mouse

                        bounds = sResumeButton.getGlobalBounds();   ///bounduri buton resume

                        ///calibrare bounduri
                        bounds.height -=5;
                        bounds.width -=5;
                        bounds.left +=5;
                        bounds.top +=5;

                        if (bounds.contains(mouse)){    ///daca click-ul dreapta cu mouse-ul e pe buton
                            isPaused = false;   ///ii spune programului sa nu mai stea in pauza
                        }
                    }
                    Event exit; ///event pt inchidere din X
                    while (app.pollEvent(exit)){    ///ia tipul de event
                    if (exit.type == Event::Closed) ///daca tipul de event este pt exit program
                        return 0;   ///se inchide programul
                    }
                    Sleep(1);
                }
                app.setFramerateLimit(80);
                app.draw(sPauseButton); ///deseneaza butonul de pauza
                pauseoutput.str("");    ///goleste ostringstream-ul, ca sa nu se genereze sir peste sir la fiecare loop de afisare
                app.display();  ///afisare elemente si loop
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)){   ///pt tasta dreapta de la keyboard
            x+=5;   ///doodle se muta 5px la dreapta
            t6.loadFromFile("images/doodlereverse.png");    ///se afiseaza imaginea cu doodle spre dreapta
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)){    ///pt tasta stanga de la keyboard
            x-=5;      ///doodle se muta 5px la stanga
            t6.loadFromFile("images/doodle.png");   ///se afiseaza imaginea cu doodle spre stanga
        }

        int i;

        for(i=0;i<2;i++) movePlats(plat3, 4, direction, i, plat1, plat2, plat4, 14, 6, 2);    ///apeleaza functia care deplaseaza platformele floating independente

        dy+=0.2;    ///la fiecare loop doodle "cade"(physics)
        y+=dy;  ///se actualizeaza pozitia globala in program
        ///if (y>500)  dy=-10; ///cred ca e useless, dar nu bag mana in foc

        if (y<h){   ///daca doodle urca
            y = h;  ///se actualizeaza y-ul

            ///se repopuleaza harta cu platforme
            generatePlats(plat1, 7, dy);
            generatePlats(plat2, 3, dy);
            generatePlats(plat3, 2, dy);
            generatePlats(plat4, 1, dy);
            overlaping_detector(plat1, plat2, plat3, plat4, 14, 6, 4, 2);
        }
        for (i=0;i<7;i++)   ///verificare daca doodle pica pe platforme normala
            if ((x+50>plat1[i].x)&&(x+20<plat1[i].x+68)&&(y+68>plat1[i].y)&&(y+68<plat1[i].y+14)&&(dy>0))
                    dy=-10;
        for (i=0;i<3;i++)   ///verificare daca doodle pica pe platforme care se distruge
            if ((x+50>plat2[i].x)&&(x+20<plat2[i].x+68)&&(y+68>plat2[i].y)&&(y+68<plat2[i].y+14)&&(dy>0))
                plat2[i].y=534; ///muta platforma care se distruge in afara afisajului si astfel spune programului sa dea render la alta platforma cu alte coordonate noi, peste cele vechi
        for (i=0;i<2;i++)   ///verificare daca doodle pica pe platforme floating
            if ((x+50>plat3[i].x)&&(x+20<plat3[i].x+68)&&(y+68>plat3[i].y)&&(y+68<plat3[i].y+14)&&(dy>0))
                    dy=-10;

        ///verificare daca doodle pica pe platforma bouncy
        if ((x+50>plat4[0].x)&&(x+20<plat4[0].x+68)&&(y+68>plat4[0].y)&&(y+68<plat4[0].y+14)&&(dy>0)){
            dy=-50; ///doodle sare de 5x mai mult
            isSpring = 1;   ///spune sistemului de afisaj ca doodle sare mai mult
        }

        sPers.setPosition(x,y); ///seteaza poz. lui doodle in plan

        app.draw(sBackground);  ///deseneaza background-ul
        app.draw(sPers);    ///deseneaza pe doodle pe ecran

        sBorder2.setPosition(0, 30);    ///seteaza pozitia border-ului de jos pt border-ul de sus
        app.draw(sBorder2); ///deseneaza border-ul

        app.draw(sBorder1); ///deseneaza border-ul in care va fi afisat text-ul cu scorul si pause button-ul, border-ul de jos va fi transparent(de aia trb imagini png la overlay)
        app.draw(sPauseButton); ///deseneaza pause button-ul

        ///deseneaza pe ecran platformele
        drawPlat(app, 7, sPlat1, plat1);
        drawPlat(app, 3, sPlat2, plat2);
        drawPlat(app, 2, sPlat3, plat3);
        drawPlat(app, 1, sPlat4, plat4);

        app.draw(text); ///deseneaza textul cu scorul
        app.display();  ///afiseaza in app tot ce a dat draw

        if(x<-40) x=390;    ///daca doodle iese din ecran prin stanga il muta in dreapta
            else if (x>390) x=-40;  ///daca doodle iese din ecran prin dreapta il muta in stanga

        if (dy>=10.76){ ///daca doodle atinge podeaua afisajului
            if (score>highScore) highScore = score; ///daca scorul curent este mai mare decat scorul maxim, se actualizeaza high score
            return 1;   ///si se inchide jocul
        }

        ///sistemul de scor
        if (y < 240 && mx > y){ ///daca se observa ca doodle urca(coord mai mici, minim 200(h-ul)), am setat 240 pt ca h + height(doodle) = 200 + 40 = 240 si am presupus ca e punctare fair
            if (mx > 240) mx = 240; ///daca mx-ul a fost actualizat cu value >240 la loop, se schimba in 240
            if (isSpring == 1) score+=5*(mx-y); ///daca doodle a sarit pe spring, scorul creste mai mult(de 5 ori mai mult decat de obicei)
            else score+=mx-y;   ///daca a fost platforma normala/floating, scorul creste normal
            mx = y;    ///se actualizeaza mx
        }
        else if (y == 200){ ///daca doodle are poz constanta(se stie ca el urca la poz asta, ca e egala cu h)
                if (isSpring == 1) score+=5;    ///daca doodle a sarit pe spring, scorul creste mai mult
                else score+=1;  ///altfel scorul creste normal
                mx = 200;   ///penultima valoare a lui y o sa fie mereu 200 in cazul asta, deci mx se actualizeaza cu 200
        }
        else if (mx == 200 && y > 200){ ///daca se observa ca doodle cade dupa ce a urcat(creste y-ul)
                isSpring = 0;   ///se reseteaza punctarea pt faptul ca doodle a sarit pe spring
                mx = y; ///se actualizeaza mx dupa y
        }

        ss.str(""); ///da clear la continutul din ss, pt a nu genera o concatenare de siruri
    }
    return 1;   ///daca se incheie jocul cu succes, se returneaza cu 1
}

bool DeathScreen(RenderWindow &app){    ///post-game screen
    Texture t1,t2,t3;  ///texturi afisaj
    t1.loadFromFile("images/death_background.png"); ///fundal
    t2.loadFromFile("images/replay.png");   ///buton replay
    t3.loadFromFile("images/quit2.png");    ///buton quit

    Sprite sBackground(t1), sReplay(t2), sQuit(t3); ///sprite-uri din texturi
    sReplay.setPosition(200, 375);  ///pozitie buton replay
    sQuit.setPosition(125, 450);    ///pozitie buton quit

    Text text_score, text_highscore, text_title;    ///text pt scorul din ultima sesiune, highscore si pt titlu;
    std::ostringstream print_score, print_highscore, print_title;    ///output pt scor, highscore.

    print_score<<"Score : "<<score; ///ce va contine output-ul pt. score
    print_highscore<<"Highscore : "<<highScore; ///ce va contine output-ul pt. highscore
    print_title<<"GAME OVER!";  ///ce va contine output-ul pt titlu

    Font font, font_title;  ///fontul pt cele 2 afisaje de scor(va fi arial, salvat local si fontul pt titlu
    if(!font.loadFromFile("res/times-new-roman.ttf")){  ///daca nu i se da load
        std::cout<<"Error loading file!"<<std::endl;    ///arunca text pt eroare in consola
        app.close();    ///inchide fereastra cu aplicatia
        return 0;   ///inchide programul
    }
    if(!font_title.loadFromFile("res/BarkingCatDEMO.otf")){  ///daca nu i se da load
        std::cout<<"Error loading file!"<<std::endl;    ///arunca text pt eroare in consola
        app.close();    ///inchide fereastra cu aplicatia
        return 0;   ///inchide programul
    }

    ///setare caract. text pt score: font, continut, size, culoare, stil, coordonate
    setcaract_text(text_score, font, print_score, 30, 30, 175);
    text_score.setFillColor(Color::Black);
    text_score.setStyle(Text::Bold);

    ///setare caract. text pt highscore: font, continut, size, culoare, stil, coordonate
    setcaract_text(text_highscore, font, print_highscore, 30, 30, 225);
    text_highscore.setFillColor(Color::Black);
    text_highscore.setStyle(Text::Bold);

    ///setare caract. text pt titlu: font, continut, size, culoare, stil, coordonate
    setcaract_text(text_title, font_title, print_title, 50, 50, 50);
    text_title.setFillColor(Color::Red);
    text_title.setStyle(Text::Bold);

    app.draw(sBackground);  ///deseneaza background-ul in app
    app.draw(sReplay);  ///deseneaza butonul de replay in app
    app.draw(sQuit);    ///deseneaza butonul de quit in app
    app.draw(text_title);   ///deseneaza textul pt titlu in app
    app.draw(text_score);   ///deseneaza textul pt. score in app
    app.draw(text_highscore);   ///desneaza textul pt. highscore in app
    app.display();  ///afiseaza in app tot ce a dat draw

    print_title.str("");    ///goleste continutul din print_title, pt a nu genera o concatenare de siruri
    print_score.str("");    ///goleste continutul din print_score, pt a nu genera o concatenare de siruri
    print_highscore.str("");    ///goleste continutul din print_highscore, pt a nu genera o concatenare de siruri
    app.setFramerateLimit(0);

    while(app.isOpen()){    ///cat timp fereastra e deschisa(asteapta actiune -- buton apasat)

        ///event pt inchidere din X
        Event e;
        while (app.pollEvent(e)){
            if (e.type == Event::Closed)
                return false;
        }

        if(Mouse::isButtonPressed(Mouse::Left)){    ///daca detecteaza left-click
            Vector2f mouse = app.mapPixelToCoords(Mouse::getPosition(app)); ///poz. mouse
            FloatRect bounds1 = sReplay.getGlobalBounds(), bounds2 = sQuit.getGlobalBounds();   ///bound-uri pt cele 2 butoane(replay si quit)
            if (bounds1.contains(mouse)){   ///daca mouse-ul e pe butonul de replay cand s-a dat left click
                return true;    ///return true(adica nu se inchide programul)
            }
            if(bounds2.contains(mouse)){    ///daca mouse-ul e pe butonul de quit cand s-a dat left click
                return false;   ///return false(se inchide programul)
            }
        }
        Sleep(1);
    }
    return false;   ///daca se inchide consola la death screen, se inchide si programul
}

int main()
{
    RenderWindow app(VideoMode(400, 533), "Doodle Game!");  ///da render pe ecran la aplicatia in sine, cu numele "Doodle Game", cu width 400px, height 533px
    app.setFramerateLimit(80);  ///aplicatia o sa aiba maxim 60fps(nu are rost mai mult)
    if (!Menu(app)){
        app.close();  ///daca se returneaza false din Meniu, se iese din aplicatie
        return 0;   ///return 0, e useless sa se mai execute restul codului, cu aplicatia inchisa
    }
    else {
        int ok=1;   ///verifica daca trebuie sau nu sa se inchida aplicatia
        while (ok)  ///cat timp aplicatia trebuie sa ruleze
            if (Game(app)){ ///daca se iese cu 1 din joc, se muta in deathscreen
                if (!DeathScreen(app)) ok=0;    ///daca se iese din deathscreen cu false, ii spune programului sa inchida aplicatia
            }
            else ok=0;  ///daca se iese cu 0 din joc(din cauza inchiderii din X) ii spune programului sa inchida aplicatia
    }
    app.close();    ///se inchide aplicatia
    return 0;   ///se inchide programul
}
