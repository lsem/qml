import QtQuick 2.4
import QtQuick.Window 2.2
import com.eleks.com.qmlcomponents 1.0;
import 'ReactiveTextController.js' as ReactiveTextController;

Window {
    id: windowObject

    width: 640
    height: 480

    visible: true

    property color reactiveTextModel: 'white'

    //color: reactiveTextModel.formColor

    MouseArea {
        id: containerMmouseArea
        anchors.fill: parent

        hoverEnabled: true
        onPositionChanged: ReactiveTextController.onReactiveTextMouseLeaved();
    }

    ReactiveTextModel {
        id: reactiveTextModel

        property int avarageAppleCaloriesCount: 72
        property int appleCount: 10
        property int totalCalories: appleCount * avarageAppleCaloriesCount

        property int appleCount_Maximum: 200
        property int appleCount_Minimum: 0
        property int appleCount_Step: 2
    }

    ReactiveText {
        id: reactiveText
        property alias thisObject: reactiveText

        model: reactiveTextModel
        text: "If you eat {{appleCount}} apples, this would result in {{totalCalories}} calories in total"

        Component.onCompleted: {
            ReactiveTextController.initialize(model, containerMmouseArea, textElement);
        }

        function onReactiveNumberHovered(x, y, modelVar) {
            ReactiveTextController.onReactiveNumberHovered(x, y, modelVar);
        }
        function onReactiveNumberPressed(x, y, modelVar) {
            ReactiveTextController.onReactiveNumberPressed(x, y, modelVar);
        }
        function onReactiveNumberRleased(x, y, modelVar) {
            ReactiveTextController.onReactiveNumberRleased(x, y, modelVar);
        }
    }

    Text {
        id: textElement

        anchors.centerIn: parent

        wrapMode: Text.WordWrap

        text: reactiveText.text

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            cursorShape: containerMmouseArea.cursorShape

            property var linkHovered

            onPressed: reactiveText.onReactiveNumberPressed(mouse.x, mouse.y, linkHovered)
            onReleased: reactiveText.onReactiveNumberRleased(mouse.x, mouse.y, linkHovered)
            onPositionChanged: {
                linkHovered = textElement.linkAt(mouse.x, mouse.y);
                reactiveText.onReactiveNumberHovered(mouse.x, mouse.y, linkHovered);
            }
        }
    }
}
