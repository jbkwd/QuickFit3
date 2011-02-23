#include "qfextensioncamera.h"
#include <QtGui>
#include <QtPlugin>
#include <iostream>

QFExtensionCameraImplementation::QFExtensionCameraImplementation(QObject* parent):
    QObject(parent)
{

}

QFExtensionCameraImplementation::~QFExtensionCameraImplementation() {

}


void QFExtensionCameraImplementation::deinit() {
	/* add code for cleanup here */
}

void QFExtensionCameraImplementation::projectChanged(QFProject* oldProject, QFProject* project) {
	/* usually cameras do not have to react to a change of the project in QuickFit .. so you don't need to do anything here 
	   But: possibly you could read config information from the project here
	 */
}

void QFExtensionCameraImplementation::initExtension() {
    /* do initializations here but do not yet connect to the camera! */
}

void QFExtensionCameraImplementation::loadSettings(ProgramOptions* settingspo) {
    QSettings& settings=*(settingspo->getQSettings());
	/* here you could read config information from the quickfit.ini file using settings object */
}

void QFExtensionCameraImplementation::storeSettings(ProgramOptions* settingspo) {
    QSettings& settings=*(settingspo->getQSettings());
	/* here you could write config information to the quickfit.ini file using settings object */
}

unsigned int QFExtensionCameraImplementation::getCameraCount() {
    return 1;
	/* how man cameras may be accessed by your plugin (e.g. if you use one driver to access several cameras */
}

void QFExtensionCameraImplementation::showSettingsWidget(unsigned int camera, QWidget* parent) {
	/* open a dialog that configures the camera.
	   
	   usually you should display a modal QDialog descendent which writes back config when the user clicks OK
	   
	   alternatively you may also display a window which stays open and allows the suer to set settings also
	   during the measurement.
	*/
}

int QFExtensionCameraImplementation::getImageWidth(unsigned int camera) {
    return /* width of the next image from the given camera */;
}

int QFExtensionCameraImplementation::getImageHeight(unsigned int camera) {
    return /* height of the next image from the given camera */;
}

bool QFExtensionCameraImplementation::isConnected(unsigned int camera) {
    return /* are we connected to the specified camera */;
}

bool QFExtensionCameraImplementation::acquire(unsigned int camera, uint32_t* data, uint64_t* timestamp) {
    if (timestamp!=NULL) {
        *timestamp=/* store a timestamp information */;
    }
	
	/*
	    acquire a new image from the given camera and store it to data
		You may expect that data is at least as large as getImageWidth(camera)*getImageHeight(camera)
	*/
}

bool QFExtensionCameraImplementation::connectDevice(unsigned int camera) {
    /* connect to the given camera */
	return true;
}

void QFExtensionCameraImplementation::disconnectDevice(unsigned int camera) {
    /* disconnect from the given camera */
}

double QFExtensionCameraImplementation::getAcquisitionTime(unsigned int camera) {
    return /* acquisition time of the last image */;
}


Q_EXPORT_PLUGIN2(/* replace with the TARGET from the .pro file (e.g. cam_testcamera) */, QFExtensionCameraImplementation)
