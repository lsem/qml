.import QtQuick 2.0 as QtQuickModuleImportedInJS

var createComponent = function(componentName, parentObject, onSuccessCb, onFailureCb) {
    var component = null;
    var instance = null;
    var finishCreation = function () {
        if (component.status === QtQuickModuleImportedInJS.Component.Ready) {
            instance = component.createObject(parentObject, { visible: false});
            if (instance === null) {
                onFailureCb();
            } else {
                onSuccessCb(instance);
            }
        } else {
            onFailureCb(m_toolTipComponent.errorString());
        }
    }

    component = Qt.createComponent(componentName);
    if (component.status === QtQuickModuleImportedInJS.Component.Ready) {
        finishCreation();
    } else {
        component.statusChanged.connect(finishCreation);
    }
}
