import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import app.address_processing 1.0
import app.address_list_model 1.0

Popup {
    id: popup
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    property int backgroundRadius: 4;

    width: 200
    height: 300
    implicitWidth: 200;
    implicitHeight: 300;
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    background: Rectangle
    {
        color:"#eeeeee";
        radius: popup.backgroundRadius;
    }
    ListView {
        anchors.centerIn: parent;
        anchors.fill: parent;
        model: addressSearchingBackend.foundWordsModel;

        delegate: Rectangle {

            id: delegateRoot;
            property WordItem wordItem: addressSearchingBackend.foundWordsModel.getWordItem(index);

            color: index === 0? "#ff8a65": "#f5f5f5";
            height: 25
            width: 200

            Text {
                text: delegateRoot.wordItem? delegateRoot.wordItem.word: "";
                font.family: roboto.name
                font.pixelSize: 24;
            }
        }
    }
    onOpened:
    {
        addressSearchingBackend.startProcessing();
    }

    enter: Transition {
              ParallelAnimation {
                  NumberAnimation {
                      property: "opacity";
                      from: 0.0;
                      to: 1.0;
                      duration: 300
                  }
                  NumberAnimation {
                      property: "scale";
                      from: 0.4;
                      to: 1.0;
                      easing.type: Easing.OutBack
                      duration: 300
                  }
                  NumberAnimation {
                      property: "backgroundRadius";
                      from: 100;
                      to: 5;
                      easing.type: Easing.OutBack
                      duration: 300
                  }
              }
          }
          exit: Transition {
               ParallelAnimation {
                   NumberAnimation {
                       property: "opacity";
                       from: 1.0
                       to: 0.0;
                       duration: 300
                   }
                   NumberAnimation {
                       property: "scale";
                       from: 1.0
                       to: 0.8;
                       duration: 300
                   }
               }
          }
}
