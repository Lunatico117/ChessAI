// Importa la clase base en Qt para aplicaciones GUI (interfaz grfica de usuario) sin widgets tradicionales
#include <QGuiApplication>

// Importa la clase necesaria para cargar y gestionar aplicaciones basadas en QML/Qt Quick
#include <QQmlApplicationEngine>

// Permite exponer objetos, variables y funciones de C++ para que sean accesibles directamente en la interfaz QML,
// definiendo el alcance (scope) de las propiedades en una jerarquia
#include <QQmlContext>

#include "../controllers/ChessController.hpp"


int main(int argc, char *argv[]) {
    // Iniciamos la aplicacion grafica
    QGuiApplication app(argc, argv);

    // Instanciamos el controlador
    ChessController chessController;

    // Iniciamos el motor de QML (El encargado de dibujar la pantalla)
    QQmlApplicationEngine engine;

    // Inyectamos nuestro controlador de C++ dentro del mundo de QML.
    // A partir de ahora, QML conocera una variable global llamada "ChessController"
    engine.rootContext()->setContextProperty("chessController", &chessController);

    // Cargamos el archivo visual principal (main.qml)
    // Usualmente en Qt6 se usa el sistema de recursos "qrc:/"
    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));

    // Es un escuchador para saber si la ventana se creo con exito
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                    // Es un funcion Lambda para saber si el objeto visual resulto nulo, si es asi cierra el programa evitando errores o crasheos
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1); // Falla si no encuentra el archivo QML
                     }, Qt::QueuedConnection);

    // Es el que empieza a construir los elementos visuales del main.qml
    engine.load(url);

    // Entregamos el control al bucle de eventos de la ventana el que se queda "escuchando" nuestros clics o movimientos
    return app.exec();
}
