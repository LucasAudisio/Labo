#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;
using namespace sf;
using namespace std::chrono;

struct enemigoClase{
    int id;
    int vida;
    int danio;
    int armaduraArco;
    int armaduraEspada;
    int x;
    int y;
    int xpAlmatarlo;
};

struct coord{
    int x;
    int y;
};

struct mapas{

    vector<vector<bool>> colisiones;
    vector<vector<int>> piso;
    vector<enemigoClase> enemigos;
};

struct estructura{
        int x;
        int y;
        int ancho;
        int alto;
};

struct itemClase{
    string ruta;
    int ancho;
    int alto;
    float desX;
    float desY;
    int danio;
    bool espada;
    int armadura;
};

class inventarioClase{
    public:
        vector<vector<int>> slots;
        Texture textura;
        Texture Cursor;
        Texture CursorMarca;
        Sprite sprite;
        Sprite CursorSprite;
        Sprite CursorMarcaSprite;
        int xCursor;
        int yCursor;
        bool entersoltado;
        bool seleccionando;
        int invX;
        int invY;
        int id_temp;
        int xtemp;
        int ytemp;
        int x;
        int y;
        bool moviendo;
        void inventarioGestion(int xJugador, int yJugador, RenderWindow &window, vector<itemClase> items){
            invY = yJugador - 32.5;
            invX = xJugador - 32.5;
            sprite.setPosition(invX, invY);

            if(!Keyboard::isKeyPressed(Keyboard::Enter) && !entersoltado){
                entersoltado = true;
            }
            else if(Keyboard::isKeyPressed(Keyboard::Enter) && !seleccionando && entersoltado){
                seleccionando = true;
                entersoltado= false;
                CursorSprite.setTextureRect(IntRect(259, 0, 259, 259));
                CursorMarcaSprite.setPosition(CursorSprite.getPosition());
                xtemp = x;
                ytemp = y;
            }
            else if(Keyboard::isKeyPressed(Keyboard::Enter) && seleccionando && entersoltado){
                seleccionando = false;
                entersoltado = false;
                CursorSprite.setTextureRect(IntRect(0,0,259,259));
                id_temp= slots[xtemp][ytemp];
                slots[xtemp][ytemp]=slots[x][y];
                slots[x][y]=id_temp;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                y--;
                if(y < 0) y = 0;
                moviendo = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                y++;
                if (y == slots.size()) y = slots.size() - 1;
                moviendo = true;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                x--;
                if (x < 0) x = 0;
                moviendo = true;
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                x++;
                if (x == slots.size()) x=slots.size() - 1;
                moviendo = true;
            }

            if(x == 0 && y == 0){
                y = 1;
            }
            else if(x == 0 && y == 3){
                y = 2;
            }

            cout << x << " " <<y <<endl;
            window.draw(sprite);
            window.draw(CursorSprite);
            if (seleccionando==true) window.draw(CursorMarcaSprite);

            for(int i = 0; i < slots.size(); i++){
                for(int j = 0; j < slots[i].size(); j++) cout << slots[i][j] << " ";
                cout << endl;
            }
            cout << "-------------" << endl;

            if(moviendo){
                moviendo = false;
                //Sleep(250);
            }


            Texture itemTextura;
            Sprite item;

            item.setScale(0.3, 0.3);

            for(int i = 0; i < slots.size(); i++){
                for(int j = 0; j < slots[i].size(); j++){
                    cout << "a" << endl;

                    if(slots[i][j] !=0){
                        itemTextura.loadFromFile(items[slots[i][j]].ruta);
                        item.setTexture(itemTextura);
                        item.setTextureRect(IntRect(0, 0, items[slots[i][j]].ancho ,items[slots[i][j]].alto));
                        item.setPosition(invX + 8 + j * 14.82 + items[slots[i][j]].desX, invY + 8 + i * 14.82 + items[slots[i][j]].desY);
                        window.draw(item);
                    }

                }
            }
            CursorSprite.setPosition(invX + 6.7 + y * 14.82, invY + 6.5 + x * 14.82);
        }
};

class playerClase{
    public:
        int x;
        int y;
        Sprite sprite;
        int xAntiguos;
        int yAntiguos;
        Texture textura;
        bool moviendo;
        steady_clock::time_point frame;
        bool pieIzqTrue;
        int movX;
        int movY;
        string direccion;
        int cont;
        int vida;
        int danio;
        int xp;
        int nivel;
        void movimiento(vector<vector<bool>> pararse){

            int velocidad = 10;
            if(Keyboard::isKeyPressed(Keyboard::LShift)) velocidad = 20;

            for(int i = 0; i < velocidad / 10; i++){

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !moviendo) {
                    moviendo = true;
                    movX = -1;
                    movY = 0;
                    frame = chrono::steady_clock::now();
                    cont = 0;
                    direccion="izq";
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !moviendo) {
                    moviendo = true;
                    movX = 1;
                    movY = 0;
                    frame = chrono::steady_clock::now();
                    cont = 0;
                    direccion="der";
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !moviendo) {
                    moviendo = true;
                    movY = -1;
                    movX = 0;
                    frame = chrono::steady_clock::now();
                    cont = 0;
                    direccion="arriba";
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !moviendo) {
                    moviendo = true;
                    movY = 1;
                    movX = 0;
                    frame = chrono::steady_clock::now();
                    cont = 0;
                    direccion="abajo";
                }
                else if (!moviendo){
                    direccion="idle";
                }

                steady_clock::time_point t2 = chrono::steady_clock::now();

                duration<double> sigFrame = duration_cast<duration<double>>(t2 - frame);

                cout << sigFrame.count() << endl;

                if(sigFrame.count() > 0.05 / (velocidad / 10) && cont < 5 && moviendo){

                    cout << sigFrame.count() << endl;

                    coord sprite_jugador={0, 0};

                    if (direccion=="izq") sprite_jugador.y=66;
                    else if (direccion=="der") sprite_jugador.y=130;
                    else if (direccion=="arriba") sprite_jugador.y=194;
                    else if (direccion=="abajo") sprite_jugador.y=0;



                    if (cont !=0 && cont != 4 && pieIzqTrue==true) sprite_jugador.x=64;
                    else if (cont !=0 && cont != 4 && pieIzqTrue==false) sprite_jugador.x=192;
                    else sprite_jugador.x=0;

                    sprite.setTextureRect(sf::IntRect(sprite_jugador.x, sprite_jugador.y, 34, 52));

                    if(movX != 0){
                        x += 2 * movX;
                    }
                    else if(movY != 0){
                        y += 2 * movY;
                    }
                    cont++;
                    frame = chrono::steady_clock::now();

                    if (cont == 5){
                        moviendo = false;
                        direccion="idle";
                        if (pieIzqTrue==true) pieIzqTrue=false;
                        else pieIzqTrue=true;
                    }

                    cout << sigFrame.count() << endl;

                    vector<coord> EsquinasCuadrante;
                    EsquinasCuadrante.push_back({x - (x % 10), y - (y % 10)}); // arriba izq
                    EsquinasCuadrante.push_back({x + 9 - ((x + 9) % 10), y - (y % 10)}); // arriba der
                    EsquinasCuadrante.push_back({x - (x % 10), y + 9 - ((y + 9) % 10)}); //abajo izq
                    EsquinasCuadrante.push_back({x + 9 - ((x + 9) % 10), y + 9 - ((y + 9) % 10)}); // abajo der

                    bool moverse = true;

                    for(int i = 0; i < 4; i++){
                        if (!pararse[EsquinasCuadrante[i].y / 10][EsquinasCuadrante[i].x / 10]) {
                            moverse = false;
                            break;
                        }
                    }
                    if(moverse) {
                        xAntiguos = x;
                        yAntiguos = y;
                    }
                    else{
                        x = xAntiguos;
                        y = yAntiguos;
                        cont = 4;
                    }

                    sprite.setPosition(x + 1, y - 3);

                }
            }
        }
};

vector<vector<int>> leerMapa(vector<vector<int>> mapa, string xd){
    ifstream mapaArchivo;

    int tile = 0;
    mapaArchivo.open(xd);
    mapa = {};
    for(int i = 0; i < 44; i++){
        mapa.push_back({});
        for(int j = 0; j < 46; j++){
            mapaArchivo >> tile;
            mapa[i].push_back(tile);
        }
    }

    mapaArchivo.close();
    return mapa;
}

vector<vector<bool>> leerColosiones(vector<vector<bool>> colisiones){
    ifstream mapaArchivo;

    int tile=0;

    mapaArchivo.open("mapaColisiones.txt");
    colisiones = {{}};
    for(int i = 0; i < 44; i++){
        colisiones.push_back({});
        for(int j = 0; j < 46; j++){
            mapaArchivo >> tile;
            if (tile == 0) colisiones[i].push_back(true);
            else colisiones[i].push_back(false);
        }
    }

    mapaArchivo.close();
    return colisiones;
}

void dibujarPiso(RenderWindow &window, vector<vector<int>> matrizPisos, int x, int y){

    Texture texturaPisos;
    texturaPisos.loadFromFile("spritesPisos.png");
    Sprite piso;
    piso.setTexture(texturaPisos);
    piso.setScale(0.67, 0.67);

    vector<coord> pisos = {
        //Caminos de tierra 0
        {18, 103 - 51},
        {35, 103 - 51},
        {52, 103 - 51},
        {69, 103 - 51},
        {18, 120 - 51},
        {35, 120 - 51},
        {52, 120 - 51},
        {69, 120 - 51},
        {18, 137 - 51},
        {35, 137 - 51},
        {52, 137 - 51},
        {69, 137 - 51},
        //Lago fachero 12
        {171, 1},
        {188, 1},
        {206, 1},
        {171, 18},
        {188, 18},
        {206, 18},
        {171, 35},
        {188, 35},
        {206, 35},
        //vallas 21
        {103, 188},
        {120, 188},
        {137, 188},
        {103, 205},
        {120, 205},
        {137, 205},
        {103, 222},
        {120, 222},
        {137, 222},
        //pasto 30
        {103, 1},
        {103, 18},
        {120, 18},
        {103, 35},
        //Arbol 34
        {239, 290},
        {256, 290},
        {239, 307},
        {256, 307},
        {239, 324},
        {256, 324},
        //Arboles 40
        {273, 290},
        {290, 290},
        {273, 307},
        {290, 307},
        {273, 324},
        {290, 324},
    };

    int yInicial = y - 100;
    int xInicial = x - 100;

    if(yInicial < 0) yInicial = 0;
    if(xInicial < 0) xInicial = 0;

    xInicial /= 10;
    yInicial /= 10;

    int xFinal = x / 10 + 11;
    int yFinal = y / 10 + 11;

    if(xFinal > matrizPisos[0].size()) xFinal = matrizPisos[0].size();

    if(yFinal > matrizPisos.size()) yFinal = matrizPisos.size();

    //cout << "xd: " << xFinal << " / " << yFinal << endl;

    for(int i = yInicial; i < yFinal; i++){
        for(int j = xInicial; j < xFinal; j++){
            piso.setTextureRect(sf::IntRect(pisos[matrizPisos[i][j]].x, pisos[matrizPisos[i][j]].y, 16, 16));
            piso.setPosition(j * 10, i * 10);
            window.draw(piso);
        }
    }
}

void dibujarEstructuras(RenderWindow &window){

    Texture texturaEstructuras;
    texturaEstructuras.loadFromFile("spritesCasas.png");
    Sprite casa;
    casa.setTexture(texturaEstructuras);
    casa.setScale(0.6, 0.64);

    vector<estructura> idEstructuras = {
        {0, 25, 81, 110}, //roja
        {0, 169, 81, 111}, //azul tejas
        {216, 26, 79, 107}, //verde
        {342, 326, 86, 121}, //azul rayas
    };

    casa.setTextureRect(sf::IntRect(idEstructuras[0].x, idEstructuras[0].y, idEstructuras[0].ancho, idEstructuras[0].alto));
    casa.setPosition(160, 90);
    window.draw(casa);
    casa.setTextureRect(sf::IntRect(idEstructuras[0].x, idEstructuras[0].y, idEstructuras[0].ancho, idEstructuras[0].alto));
    casa.setPosition(310, 140);
    window.draw(casa);
    casa.setTextureRect(sf::IntRect(idEstructuras[1].x, idEstructuras[1].y, idEstructuras[1].ancho, idEstructuras[1].alto));
    casa.setPosition(250, 90);
    window.draw(casa);
    casa.setTextureRect(sf::IntRect(idEstructuras[1].x, idEstructuras[1].y, idEstructuras[1].ancho, idEstructuras[1].alto));
    casa.setPosition(310, 230);
    window.draw(casa);
    casa.setTextureRect(sf::IntRect(idEstructuras[2].x, idEstructuras[2].y, idEstructuras[2].ancho, idEstructuras[2].alto));
    casa.setPosition(100, 140);
    casa.setScale(0.57, 0.64);
    window.draw(casa);
    casa.setTextureRect(sf::IntRect(idEstructuras[3].x, idEstructuras[3].y, idEstructuras[3].ancho, idEstructuras[3].alto));
    casa.setPosition(100, 230);
    casa.setScale(0.54, 0.6);
    window.draw(casa);
}

void dibujarEnemigos(RenderWindow &window, vector<enemigoClase> TodosLosEnemigos, vector<string> idEnemigos){

}

enemigoClase checkearEnemigos(vector<enemigoClase> &TodoslosEnemigos, int xjugador, int yjugador, bool &hayUnEnemigo){
    int xd = 0;
    hayUnEnemigo = false;
    cout << xjugador << " / " << yjugador << endl;
    cout << TodoslosEnemigos[0].x << " / " << TodoslosEnemigos[0].y << endl;
    cout << "-----------------------------" << endl;
    for(int i = 0; i < TodoslosEnemigos.size(); i++){
        if(xjugador == TodoslosEnemigos[i].x && yjugador == TodoslosEnemigos[i].y){
            xd = i;
            hayUnEnemigo = true;
            cout << "aaaaaaa" << endl;
            break;
        }
    }
    if(hayUnEnemigo){
         TodoslosEnemigos[xd].x = -1;
         TodoslosEnemigos[xd].y = -1;
    }
    return TodoslosEnemigos[xd];
}

void defecarse_A_Pinias(playerClase &Jugador, enemigoClase &Enemigo, vector<int> inventario, vector <itemClase> TodosLosItems, RenderWindow &window){
    bool turnoJugador = true;
    int defensaInicial = TodosLosItems[inventario[1]].armadura;
    int danioInicial   = TodosLosItems[inventario[0]].danio;
    int vidaInicial    = Jugador.vida;
    vector<vector<string>> opciones = {
        {"atacar", "defender"},
        {"picarse", "vendaje"},
    };
    int opcionFila = 0;
    int opcionColumna = 0;
    while(Jugador.vida > 0 && Enemigo.vida > 0){
        int danio = 0;
        if(turnoJugador){
            cin >> opcionFila;
            cin >> opcionColumna;
            turnoJugador = false;
            if(opciones[opcionFila][opcionColumna] == "atacar"){//drakultop
                if(TodosLosItems[inventario[1]].espada){
                    danio = (Jugador.danio + TodosLosItems[inventario[1]].danio) - Enemigo.armaduraEspada;
                }
                else{
                    danio = (Jugador.danio + TodosLosItems[inventario[1]].danio) - Enemigo.armaduraEspada;
                }
                if(danio < 0) danio = 0;
                Enemigo.vida -= danio;
            }

            else if(opciones[opcionFila][opcionColumna] == "defender"){
                TodosLosItems[inventario[1]].armadura += 60 * TodosLosItems[inventario[1]].armadura / 100;
            }

            else if(opciones[opcionFila][opcionColumna] == "picarse"){
                TodosLosItems[inventario[0]].danio += 10 * TodosLosItems[inventario[0]].danio / 100;
            }

            else if(opciones[opcionFila][opcionColumna] == "vendaje"){
                Jugador.vida += 20 * vidaInicial / 100;
                if(Jugador.vida > vidaInicial) Jugador.vida = vidaInicial;
            }
        }
        else{
            turnoJugador = true;
            TodosLosItems[inventario[1]].armadura = defensaInicial;
        }
    }
    if(Jugador.vida > 0){
        Jugador.vida = vidaInicial;
        TodosLosItems[inventario[0]].danio = danioInicial;
        TodosLosItems[inventario[1]].armadura = defensaInicial;
        Jugador.xp += Enemigo.xpAlmatarlo;
        while(Jugador.xp > 50){
            Jugador.xp -= 50;
            Jugador.nivel++;
            Jugador.vida += 5 * Jugador.vida / 100;
            Jugador.danio += 5 * Jugador.danio / 100;
        }

    }
    else{

    }
}

int main()
{
    RenderWindow window(sf::VideoMode(1000, 1000), "El DitoGame");
    //window.setFramerateLimit(10);
    //drakultop
    View camara;

    bool inv = false, TeclaSoltada = false;

    //inicializar camara

    camara.setSize(200, 200);
    camara.setCenter(10, 10);
    window.setView(camara);


    //inicializar jugador

    playerClase Player;
    Player.textura.loadFromFile("sprites.png");
    Player.sprite.setTexture(Player.textura);
    Player.sprite.setScale(0.25, 0.25);
    Player.x = 150;
    Player.y = 140;
    Player.xAntiguos = Player.x;
    Player.yAntiguos = Player.y;
    Player.sprite.setTextureRect(sf::IntRect(0, 0, 34, 52));
    Player.moviendo = false;
    Player.sprite.setPosition(Player.x + 1, Player.y - 3);
    Player.vida = 100;
    Player.danio = 20;
    //Player.frame = chrono::steady_clock::now();
    //ditomaso y drakul20
    //Inicializar inventario


    inventarioClase inventario;
    inventario.textura.loadFromFile("inv_1.png");
    inventario.Cursor.loadFromFile("inv_seleccion.png");
    inventario.CursorMarca.loadFromFile("inv_seleccion.png");
    inventario.CursorMarcaSprite.setTexture(inventario.CursorMarca);
    inventario.sprite.setTexture(inventario.textura);
    inventario.CursorSprite.setTexture(inventario.Cursor);
    inventario.CursorSprite.setTextureRect(IntRect(0,0,259,259));
    inventario.CursorMarcaSprite.setTextureRect(IntRect(259,0,259,259));
    inventario.sprite.setScale(0.05, 0.05);
    inventario.CursorSprite.setScale(0.05, 0.05);
    inventario.CursorMarcaSprite.setScale(0.05, 0.05);
    inventario.moviendo = false;

    inventario.x = 0;
    inventario.y = 1;

    inventario.slots = {
        {0,1,2,0},
        {3,4,5,6},
        {7,8,9,0},
        {0,0,0,0},
    };
//drakultop
    vector<itemClase> items = {
        {"vacio.png", 0, 0, 0, 0, 0, false, 0},
        {"espadaFundida.png", 34, 34, 0.5, 0.1, 100, true, 0},
        {"espadaMadera.png", 32, 32, 0.5, 0, 20, true, 0},
        {"espadaHierro.png", 32, 32, 0.5, 0.1, 50, true, 0},
        {"armaduraNinja.png", 30, 18, 0.5, 2, 0, false, 10},
        {"armaduraHierro.png", 30, 19, 1, 2, 0, false, 30},
        {"armaduraFundida.png", 26, 18, 1.5, 2, 0, false, 60},
        {"arcoMadera.png", 16, 32, 3, 0, 20, false, 0},
        {"arcoHierro.png", 16, 32, 3, 0, false, 0},
        {"arcoFundido.png", 18, 36, 3, -0.5, 100, false, 0},
    };

    //inicializar mapa

    mapas mapa;
    mapa.colisiones = leerColosiones(mapa.colisiones);
    mapa.enemigos = {
        {0, 30, 15, 6, 8, 100, 390, 5},
    };
    mapa.piso = leerMapa(mapa.piso, "mapaPiso.txt");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(!Keyboard::isKeyPressed(Keyboard::I) && !TeclaSoltada){
            TeclaSoltada = true;
        }
        else if(Keyboard::isKeyPressed(Keyboard::I) && !inv && TeclaSoltada){
            inv = true;
            TeclaSoltada = false;
        }
        else if(Keyboard::isKeyPressed(Keyboard::I) && inv && TeclaSoltada){
            inv = false;
            TeclaSoltada = false;
        }

        bool hayEnemigo = false;

        camara.setCenter(Player.x, Player.y);
        window.setView(camara);
        window.clear();
        dibujarPiso(window, mapa.piso, Player.x, Player.y);
        dibujarEstructuras(window);
        window.draw(Player.sprite);

        enemigoClase enemigoEncontrado = checkearEnemigos(mapa.enemigos, Player.x, Player.y, hayEnemigo);


//ditomaso
        if(inv){
            inventario.inventarioGestion(Player.x, Player.y, window, items);
        }
        else if(hayEnemigo){
            cout << "drakulPuto" << endl;
            cin >> Player.x;
        }
        else Player.movimiento(mapa.colisiones);
        window.display();
        //cout << Player.x << " / " << Player.y << "a" << endl;
    }
    return 0;
}
//drakultop
