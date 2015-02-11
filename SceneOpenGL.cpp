#include "SceneOpenGL.h"


// Permet d'�viter la r�-�criture du namespace glm::

using namespace glm;


// Constructeur de Destucteur

SceneOpenGL::SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) : m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
                                                                                             m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0),
                                                                                             m_input()
{

}


SceneOpenGL::~SceneOpenGL()
{
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}


// M�thodes

bool SceneOpenGL::initialiserFenetre()
{
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Version d'OpenGL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Double Buffer

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Cr�ation de la fen�tre

    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(m_fenetre == 0)
    {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return false;
    }


    // Cr�ation du contexte OpenGL

    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);

    if(m_contexteOpenGL == 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();

        return false;
    }

    return true;
}


bool SceneOpenGL::initGL()
{
    #ifdef WIN32

        // On initialise GLEW

        GLenum initialisationGLEW( glewInit() );


        // Si l'initialisation a �chou� :

        if(initialisationGLEW != GLEW_OK)
        {
            // On affiche l'erreur gr�ce � la fonction : glewGetErrorString(GLenum code)

            std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;


            // On quitte la SDL

            SDL_GL_DeleteContext(m_contexteOpenGL);
            SDL_DestroyWindow(m_fenetre);
            SDL_Quit();

            return false;
        }

    #endif


    // Activation du Depth Buffer

    glEnable(GL_DEPTH_TEST);


    // Tout s'est bien pass�, on retourne true

    return true;
}


void SceneOpenGL::bouclePrincipale()
{
   // Variables

    Uint32 frameRate (1000 / 50);
    Uint32 debutBoucle(0), finBoucle(0), tempsEcoule(0);


    // Matrices

    mat4 projection;
    mat4 modelview;

    projection = perspective(70.0, (double) m_largeurFenetre / m_hauteurFenetre, .5, 200.0);
    modelview = mat4(1.0);


    // Cam�ra mobile

    Camera camera(vec3(20, 20, 20), vec3(0, 0, 0), vec3(0, 1, 0), 0.5, 0.5);
    m_input.afficherPointeur(false);
    m_input.capturerPointeur(true);


    // Vertices

    float vertices[] = {-100, 0, -100,   100, 0, -100,   100, 0, 100,    // Triangle 1
                        -100, 0, -100,   -100, 0, 100,   100, 0, 100};   // Triangle 2



    // Coordonn�es de texture

    float coordTexture[] = {0, 0,   7, 0,   7, 7,     // Triangle 1
                            0, 0,   0, 7,   7, 7};    // Triangle 2


    // Texture

    Texture texture("Textures/Herbe.jpg");
    texture.charger();


    // Shader

    Shader shaderTexture("Shaders/texture.vert", "Shaders/texture.frag");
    shaderTexture.charger();


    // Objet Caisse

    Caisse caisse(50.0, "Shaders/texture.vert", "Shaders/texture.frag", "Textures/Caisse2.jpg");

    Cube cube(5.0, "Shaders/couleur3D.vert", "Shaders/couleur3D.frag");


    // Boucle principale

    while(!m_input.terminer())
    {
        // On d�finit le temps de d�but de boucle

        debutBoucle = SDL_GetTicks();


        // Gestion des �v�nements

        m_input.updateEvenements();

        if(m_input.getTouche(SDL_SCANCODE_ESCAPE))
           break;

        camera.deplacer(m_input);


//cube commande
        if(m_input.getTouche(SDL_SCANCODE_O))
        {
            cube.setPosition(cube.getPosition() + vec3(0, 0.2, 0));
        }
        else
        {
            float y_plus=0.0, z_plus=cube.getPosition().z,taille_2,alpha_x;
            alpha_x=cube.getAngle().x*M_PI/180;
            taille_2=cube.getTaille().y/2.0;
            if(((alpha_x > 0)&&(alpha_x < M_PI/2.0))||((alpha_x > M_PI)&&(alpha_x < 3*M_PI/2.0))){
                y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)+alpha_x));
            }
            else{
                y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)-alpha_x));
            }

            if(cube.getPosition().y-(y_plus)>0){
                    //on d�cr�mente sur y tanque on ateint pas le sol
                cube.setPosition(cube.getPosition() - vec3(0.0, 0.05, 0.0));
            }
            else{
                if(y_plus-(cube.getTaille().y/2.0)>0){
                    //std::cout << "ok" << std::endl;
                    //m�thode des angle � r�fl�chir m�thode de d�cr�mentation sur y
                    if(((alpha_x > 0)&&(alpha_x < M_PI/4.0))||((alpha_x > M_PI)&&(alpha_x < 5*M_PI/4.0))){
                        cube.setAngle(cube.getAngle() - vec3(0.4, 0, 0));
                        z_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)+alpha_x));
                        alpha_x=cube.getAngle().x*M_PI/180;
                        z_plus=cube.getPosition().z+(z_plus-abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)+alpha_x)));
                        y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)+alpha_x));
                    }
                    else if(((alpha_x > M_PI/4.0)&&(alpha_x < M_PI/2.0))||((alpha_x > 5*M_PI/4.0)&&(alpha_x < 3*M_PI/2.0))){
                        cube.setAngle(cube.getAngle() + vec3(0.4, 0, 0));
                        z_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)+alpha_x));
                        alpha_x=cube.getAngle().x*M_PI/180;
                        z_plus=cube.getPosition().z-(z_plus-abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)+alpha_x)));
                        y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)+alpha_x));
                    }
                    else if(((alpha_x > M_PI/2.0)&&(alpha_x < 3*M_PI/4.0))||((alpha_x > 3*M_PI/2.0)&&(alpha_x < 7*M_PI/4.0))){
                        cube.setAngle(cube.getAngle() - vec3(0.4, 0, 0));
                        z_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)-alpha_x));
                        alpha_x=cube.getAngle().x*M_PI/180;
                        z_plus=cube.getPosition().z+(z_plus-abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)-alpha_x)));
                        y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)-alpha_x));
                    }
                    else if(((alpha_x > 3*M_PI/4.0)&&(alpha_x < M_PI))||((alpha_x > 7*M_PI/4.0)&&(alpha_x < 2*M_PI))){
                        cube.setAngle(cube.getAngle() + vec3(0.4, 0, 0));
                        z_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)-alpha_x));
                        alpha_x=cube.getAngle().x*M_PI/180;
                        z_plus=cube.getPosition().z-(z_plus-abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*cos((M_PI/4)-alpha_x)));
                        y_plus=abs(sqrt((taille_2*taille_2)+(taille_2*taille_2))*sin((M_PI/4)-alpha_x));
                    }
                }
                cube.setPosition(vec3(0.0, y_plus, z_plus));
            }
        }

        if(m_input.getTouche(SDL_SCANCODE_Y))
        {
            cube.setAngle(cube.getAngle() + vec3(0.4, 0, 0));
        }
        if(m_input.getTouche(SDL_SCANCODE_H))
        {
            cube.setAngle(cube.getAngle() - vec3(0.4, 0, 0));
        }
        if(m_input.getTouche(SDL_SCANCODE_U))
        {
            cube.setAngle(cube.getAngle() + vec3(0, 0, 0.4));
        }
        if(m_input.getTouche(SDL_SCANCODE_J))
        {
           cube.setAngle(cube.getAngle() - vec3(0, 0, 0.4));
        }

        //fin commande cube
        // Nettoyage de l'�cran

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Gestion de la cam�ra

        camera.lookAt(modelview);


        // Sauvegarde de la matrice modelview

       // caisse.afficher(projection,modelview);

        cube.afficher(projection,modelview);


        // Activation du shader

        glUseProgram(shaderTexture.getProgramID());


            // Envoi des vertices

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);


            // Envoi des coordonn�es de texture

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, coordTexture);
            glEnableVertexAttribArray(2);


            // Envoi des matrices

            glUniformMatrix4fv(glGetUniformLocation(shaderTexture.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shaderTexture.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));


            // Verrouillage de la texture

            glBindTexture(GL_TEXTURE_2D, texture.getID());


            // Rendu

            glDrawArrays(GL_TRIANGLES, 0, 6);


            // D�verrouillage de la texture

            glBindTexture(GL_TEXTURE_2D, 0);


            // D�sactivation des tableaux

            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(0);


        // D�sactivation du shader

        glUseProgram(0);


        // Actualisation de la fen�tre

        SDL_GL_SwapWindow(m_fenetre);


        // Calcul du temps �coul�

        finBoucle = SDL_GetTicks();
        tempsEcoule = finBoucle - debutBoucle;


        // Si n�cessaire, on met en pause le programme

        if(tempsEcoule < frameRate)
            SDL_Delay(frameRate - tempsEcoule);
    }
}

