#include <QGuiApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QTexture>

int main(int argc, char *argv[])
{
    QGuiApplication application(argc, argv);
    Qt3DExtras::Qt3DWindow window;

    // root entity
    auto scene = new Qt3DCore::QEntity;
    window.setRootEntity(scene);

    // entity
    auto powerup = new Qt3DCore::QEntity(scene);

    // mesh
    Qt3DRender::QMesh* mesh = new Qt3DRender::QMesh(scene);
    mesh->setSource(QUrl::fromLocalFile(":/powerup.obj"));

    // material
    auto material = new Qt3DExtras::QDiffuseSpecularMaterial(powerup);
    material->setAmbient(Qt::white);
    material->setShininess(200.f);
    material->setAlphaBlendingEnabled(false);
    material->setTextureScale(1.0f);
    material->setSpecular(QVariant::fromValue<QColor>(Qt::lightGray));

    auto tl = new Qt3DRender::QTextureLoader(powerup);
    QObject::connect(tl, &Qt3DRender::QTextureLoader::sourceChanged,
                     [=](){qDebug() << "QTextureLoader: sourceChanged"; });
    QObject::connect(tl, &Qt3DRender::QTextureLoader::statusChanged,
                     [=](Qt3DRender::QAbstractTexture::Status s)
                     {qDebug() << "QTextureLoader: status =" << s; });
    tl->setMirrored(true);
    tl->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
    tl->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    tl->setFormat(Qt3DRender::QAbstractTexture::Automatic);

    Qt3DRender::QTextureWrapMode wm;
    wm.setX(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setY(Qt3DRender::QTextureWrapMode::ClampToEdge);
    wm.setZ(Qt3DRender::QTextureWrapMode::ClampToEdge);
    tl->setWrapMode(wm);
    tl->setSource(QUrl::fromLocalFile(":/powerup.png"));
    material->setDiffuse(QVariant::fromValue<Qt3DRender::QTextureLoader*>(tl));

    // transform
    auto transform = new Qt3DCore::QTransform;
    transform->setTranslation(QVector3D(0.0f, 0.0f, -1.0f));
    transform->setScale3D(QVector3D(10, 10, 10));

    powerup->addComponent(transform);
    powerup->addComponent(material);
    powerup->addComponent(mesh);

    // camera
    auto camera = window.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.0f, 0.0f, 100.0f));
    camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
    auto camController = new Qt3DExtras::QFirstPersonCameraController(scene);
    camController->setCamera(camera);

    window.show();
    return application.exec();
}
