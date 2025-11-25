#include "RentalSystem.h"
#include "database.h"
#include <QApplication>
#include <QStyleFactory>

#include <memory>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    
    Database db;
    db.dbInit();
    
    RentalSystem sys(db);

    sys.show();
    
    return app.exec();; 
}