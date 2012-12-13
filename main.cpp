#include "Lanceur/Lanceur.h"
#include "Tools/debug.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);
    boost::shared_ptr<Lanceur> fenetre(new Lanceur());
    app.setStyleSheet("QFrame#cocorico {background-image:url(Images/fondLanceur1.jpg);}");

    fenetre->show();

    return app.exec();
}
