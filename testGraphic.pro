SOURCES += \
    main.cpp \
    \
    Tools\Debug.cpp \
    \
    FenetrePrincipale\ActionNext.cpp \
    FenetrePrincipale\BoutonGoban.cpp \
    FenetrePrincipale\FenetreInfos.cpp \
    \
    Partie\Partie.cpp \
    Partie\Joueur.cpp \
    \
    GobanFiles\Goban.cpp \
    GobanFiles\Pierre.cpp \
    GobanFiles\Groupe.cpp \
    Partie/Coup.cpp \
    Tools/CoupException.cpp \
    Lanceur/Lanceur.cpp \
    FenetrePrincipale/FenetrePrincipale.cpp \
    FenetrePrincipale/FenetreSGF.cpp \
    FenetrePrincipale/InfosJoueur.cpp \
    GobanFiles/GobanManager.cpp \
    Partie/PartieManager.cpp





HEADERS += \
    main.h \
    \
    Tools\debug.h \
    \
    FenetrePrincipale/FenetrePrincipale.h \
    FenetrePrincipale/FenetreJeu.h \
    FenetrePrincipale/InfosJoueur.h \
    FenetrePrincipale/FenetreSGF.h \
    FenetrePrincipale\ActionNext.h \
    FenetrePrincipale\BoutonGoban.h \
    FenetrePrincipale\FenetreInfos.h \
    \
    Partie\Partie.h \
    Partie\Joueur.h \
    Partie/Coup.h \
    \
    GobanFiles\Goban.h \
    GobanFiles\Pierre.h \
    GobanFiles\Groupe.h \
    \
    Tools/CoupException.h \
    \
    Lanceur/Lanceur.h \
    GobanFiles/GobanManager.h \
    Partie/PartieManager.h


FORMS +=

INCLUDEPATH += 'C:\Program Files (x86)\boost\boost_1_51_0'
