#include <iostream>
#include <QtWidgets/QApplication>
#include "model/standardUser.h"
#include "model/cloudModel.h"
#include "model/user.h"
#include "view/cloudView.h"

using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
    QApplication *app = new QApplication(argc, argv);
    cloudModel* model = new cloudModel();
    cloudView view(model);
    view.show();
    return app->exec();

}
