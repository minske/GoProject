#include "FP.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);
    boost::shared_ptr<FP> fenetre(new FP());
    fenetre->show();

    return app.exec();
}
