#include "main.h"

int main(int argc, char** argv)
{
    srand(time(NULL));
    /*freopen("CON","w",stdout);
    freopen("erreurs.txt","w",stderr);*/

    struct SimpleCapParams capture = init_camera(320,240);
    SDL_Surface* images[NB_IMG] = {NULL,NULL,NULL};
    SDL_Event event;
    SDL_Rect pos = {86,46,0,0},pos2 = {220,46,0,0},pos3 = {86,180,0,0},pos4 = {220,180,0,0};
    Uint32 pixel[240][320];

    images[0] = init(LARGEUR,HAUTEUR,"Eyekey",0);
    images[1] = s_IMG_Load("ressources/masque.gif");
    images[2] = s_IMG_Load("ressources/masque_correct.png");

    degrade(ecran);

    SDL_InputBox* InputBox = NULL,*InputBox2 = NULL,*InputBox3 = NULL;
    SDL_Button* button = NULL,*button2 = NULL,*button3 = NULL;


    SDL_Rect dim = {60,270,200,0},dim2 = {60,315,200,0},
             dim3 = {60,395,50,0},dim4 = {156,395,50,0},
             dim6 = {60,360,200,0},
             dim7 = {110,220,100,0};

    InputBox =  SDL_CreateInputBox(dim,NOIR,BLANC,15,512,25,"ressources/LinBiolinum_R_G.ttf",2,0);
    InputBox2 = SDL_CreateInputBox(dim2,NOIR,BLANC,15,512,25,"ressources/LinBiolinum_R_G.ttf",2,0);
    InputBox3 = SDL_CreateInputBox(dim6,NOIR,BLANC,15,512,25,"ressources/LinBiolinum_R_G.ttf",2,1);
    if (argc > 1)
        strncpy(InputBox->buffer,argv[1],strlen(argv[1]));
    button =    SDL_CreateButton(dim3,"  encrypt  ","ressources/LinBiolinum_R_G.ttf",20,NOIR,GRIS);
    button2 =   SDL_CreateButton(dim4,"  decrypt  ","ressources/LinBiolinum_R_G.ttf",20,NOIR,GRIS),
    button3 =   SDL_CreateButton(dim7,"  generate code  ","ressources/LinBiolinum_R_G.ttf",15,NOIR,GRIS);

    TTF_Font* font = s_TTF_OpenFont("ressources/LinBiolinum_R_G.ttf",14);

    degrade(ecran);

    if (!balance_des_blancs(ecran,pixel,capture,InputBox,InputBox2,InputBox3,button3,font))
        goto fin;

    afficher_console(ecran,"Position the four corners in the places indicated");

    while(event.type != SDL_QUIT)
    {
        SDL_PollEvent(&event);
        if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_QUIT))
            goto fin;

        afficher(ecran,pixel,capture,1,1);
        SDL_BlitSurface(images[1],NULL,ecran,&pos);

        corrects = 0;

        if (correct2(86,100,46,60,pixel,90)
        && !correct2(101,115,46,60,pixel,60)
        && !correct2(86,100,61,75,pixel,60)
        && !correct2(101,115,61,75,pixel,60))
        {
            SDL_BlitSurface(images[2],NULL,ecran,&pos2);
            corrects++;
        }
        if (correct2(219,234,46,60,pixel,90)
        && !correct2(204,219,46,60,pixel,60)
        && !correct2(219,234,61,75,pixel,60)
        && !correct2(204,219,61,75,pixel,60))
        {
            SDL_BlitSurface(images[2],NULL,ecran,&pos);
            corrects++;
        }
        if (correct2(86,100,179,193,pixel,90)
        && !correct2(101,115,179,193,pixel,60)
        && !correct2(86,100,164,178,pixel,60)
        && !correct2(101,115,164,178,pixel,60))
        {
            SDL_BlitSurface(images[2],NULL,ecran,&pos4);
            corrects++;
        }
        if (correct2(219,234,179,193,pixel,90)
        && !correct2(204,219,179,193,pixel,60)
        && !correct2(219,234,164,178,pixel,60)
        && !correct2(204,219,164,178,pixel,60))
        {
            SDL_BlitSurface(images[2],NULL,ecran,&pos3);
            corrects++;
        }

        SDL_Flip(ecran);

        if (corrects == 4)
        {
            deinitCapture(0);
            afficher_console(ecran,"Code reading performed");
            int tableau[10][10],x,y,i,j,clignotement;
            memset(tableau,0,sizeof(int)*100);



            for (clignotement = 0 ; clignotement < 4 ; clignotement++){

            for (i = 0 ; i < 320 ; i++)
                for (j = 0 ; j < 240 ; j++)
                        definirPixel(ecran,319-i,j,pixel[j][i]);
            SDL_BlitSurface(images[2],NULL,ecran,&pos);
            SDL_BlitSurface(images[2],NULL,ecran,&pos2);
            SDL_BlitSurface(images[2],NULL,ecran,&pos3);
            SDL_BlitSurface(images[2],NULL,ecran,&pos4);
            SDL_Flip(ecran);
            SDL_Delay(250);

            for (y = 0 ; y < 10 ; y++)
            {
                for (x = 9 ; x > -1 ; x--)
                {
                    i = 86 + x*15;
                    j = 46 + y*15;
                    tableau[x][y] = correct(i,i+15,j,j+15,pixel,ecran,font);
                }
            }
            SDL_Flip(ecran);
            SDL_Delay(350);

            }

            afficher_console(ecran,"Complete the fields above");
            SDL_Flip(ecran);

            tableau[0][0] = -1; tableau[0][1] = -1; tableau[1][0] = -1; tableau[1][1] = -1;
            tableau[9][9] = -1; tableau[9][8] = -1; tableau[8][9] = -1; tableau[8][8] = -1;
            tableau[0][9] = -1; tableau[0][8] = -1; tableau[1][9] = -1; tableau[1][8] = -1;
            tableau[9][0] = -1; tableau[8][0] = -1; tableau[9][1] = -1; tableau[8][1] = -1;

            unsigned char chaine[85];
            memset(chaine,0,85);
            int c = 0;

            for (y = 0 ; y < 10 ; y++)
                for (x = 9 ; x > -1 ; x--)
                    if (tableau[x][y] != -1)
                        chaine[c++] = tableau[x][y] + '0';


            int n;
            char cle[10][9];
            memset(cle,0,90);

            for (i = 0 ; i < 10 ; i++)
            {
                n = byteToNum(chaine+8*i);
                sprintf(cle[i],"%d",n%40230);
            }

            int suite = 1;
            do
            {
                SDL_WaitEvent(&event);

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                    goto fin;

                SDL_GetButtonState(button,event);
                SDL_RefreshButton(button,ecran);
                SDL_GetButtonState(button2,event);
                SDL_RefreshButton(button2,ecran);
                actualiser(event,InputBox,InputBox2,InputBox3,ecran);
                SDL_Flip(ecran);

                if ((button->state == 1) && InputBox->bufflen && InputBox2->bufflen)
                {
                    if (!mdp_ok(InputBox3->bufflen))
                    {
                        afficher_console(ecran,"Invalid password, please enter another");
                        InputBox3->buffer[0] = '\0';
                        InputBox3->bufflen = 0;
                    }
                    else
                    {
                        char msg_crypt[256];

                        afficher_console(ecran,"Encryption in progress");

                        size_t taille_f;
                        unsigned char* f = charger_fichier(InputBox->buffer,&taille_f);

                        xor_crypter(f,chaine[80],taille_f);
                        xor_crypter(f,chaine[81],taille_f);

                        for (i = 0 ; i < 10 ; i++)
                            crypter(f,cle[i],taille_f);

                        xor_crypter(f,chaine[82],taille_f);

                        xor_crypter(InputBox3->buffer2,(chaine[20]+chaine[40]+chaine[60])%256,InputBox3->bufflen);
                        vigenere(f,InputBox3->buffer2,taille_f,InputBox3->bufflen,0);

                        xor_crypter(f,chaine[83],taille_f);

                        enregistrer_fichier(f,InputBox2->buffer,taille_f);
                        free(f);
                        suite = 0;

                        sprintf(msg_crypt,"Encryption performed");

                        afficher_console(ecran,msg_crypt);

                        goto fin2;

                    }
                }
                else if (button2->state == 1 && InputBox->bufflen && InputBox2->bufflen)
                {
                     if (!mdp_ok(InputBox3->bufflen))
                    {
                        afficher_console(ecran,"Invalid password, please enter another");
                        InputBox3->buffer[0] = '\0';
                        InputBox3->bufflen = 0;
                    }
                    else
                    {
                        afficher_console(ecran,"Decryption in progress ...");
                        size_t taille_f;
                        unsigned char* f = charger_fichier(InputBox->buffer,&taille_f);

                        xor_crypter(f,chaine[83],taille_f);

                        xor_crypter(InputBox3->buffer2,(chaine[20]+chaine[40]+chaine[60])%256,InputBox3->bufflen);
                        vigenere(f,InputBox3->buffer2,taille_f,InputBox3->bufflen,1);

                        xor_crypter(f,chaine[82],taille_f);

                        for (i = 0 ; i < 10 ; i++)
                            decrypter(f,cle[9-i],taille_f);

                        xor_crypter(f,chaine[81],taille_f);
                        xor_crypter(f,chaine[80],taille_f);

                        enregistrer_fichier(f,InputBox2->buffer,taille_f);
                        free(f);
                        suite = 0;
                        afficher_console(ecran,"Decryption completed");
                    }
                }

                SDL_Flip(ecran);
                SDL_Delay(25);

            }while(suite && event.type != SDL_QUIT);

            goto fin;
        }
    }

    /// fin ///

    fin:
    afficher_console(ecran,"Press Esc to exit");
    pause();
    fin2:
    deinitCapture(0);
    SDL_FreeInputBox(InputBox);
    SDL_FreeInputBox(InputBox2);
    SDL_FreeInputBox(InputBox3);
    SDL_FreeButton(button);
    SDL_FreeButton(button2);
    SDL_FreeButton(button3);
    sauv_blanc();
    TTF_CloseFont(font);
    TTF_Quit();
    cleanup(images);
    return EXIT_SUCCESS;
}
