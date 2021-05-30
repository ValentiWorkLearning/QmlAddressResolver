import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.5

import Qt.labs.qmlmodels 1.0
import AddressModelModule 1.0
import address.storage 1.0

ApplicationWindow {
    id: appRoot;
    width: 800
    height: 680
    visible: true
    title: qsTr("Address resolver")
    color: "#cfcfcf";

    FontLoader {
        id: roboto
        source: "qrc:/resources/Roboto.ttf"
    }

    AddressModelHandler
    {
        id:addressHandler;
    }

    AddressStorageModel
    {
        id: addressStorage;
    }

    ColumnLayout
    {
        anchors.fill: parent;

        RowLayout
        {
            Layout.preferredWidth: appRoot.width;
            Layout.preferredHeight: appRoot.height;

            GridView {
                id: itemsGridView
                Layout.preferredWidth: appRoot.width * 0.6;
                Layout.preferredHeight: appRoot.height * 0.6;
                clip: true

                model: 100

                cellWidth: 45
                cellHeight: 45

                delegate: numberDelegate
            }

            Component {
                id: numberDelegate

                Rectangle {
                    id: delegateRect
                    width: 40
                    height: 40
                    radius: 4;
                    state: "default";

                    MouseArea
                    {
                        property string delegateState;

                        anchors.fill: parent;
                        onPressed:
                        {
                            delegateState = delegateRect.state;
                            delegateRect.state="pressed";
                        }
                        onClicked:
                        {
                            addressHandler.onItemClicked(index,index);

                            if( !addressHandler.containsValue(index) )
                                delegateRect.state = "default";
                        }
                        onCanceled:
                        {
                            delegateRect.state = delegateState;
                        }

                        Connections
                        {
                            target: addressHandler;
                            function onOnItemHiglhightSetRequested(itemId,addressPosition) {
                                if(itemId === index)
                                    delegateRect.state = "checked";
                            }
                            function onOnItemHiglhightResetRequested(itemId,addressPosition){
                                if(itemId === index)
                                    delegateRect.state = "default";
                            }
                        }
                    }
                    Text {
                        anchors.centerIn: parent;
                        text: index;
                        font.family: roboto.name;
                    }

                    states:[
                        State {
                            name: "default"
                            PropertyChanges {
                                target: delegateRect
                                color: "#efebe9"
                            }
                        },
                        State {
                            name: "pressed"
                            PropertyChanges {
                                target: delegateRect
                                color: "#bdb9b7"
                            }
                        },
                        State {
                            name: "checked"
                            PropertyChanges {
                                target: delegateRect
                                color:"#ffab91"
                            }
                        }
                    ]
                }
            }


            TableModel {
                id: testTableModel;

                TableModelColumn { display: "address" }
                TableModelColumn { display: "count" }

                rows: [
                    {
                        "address": "12:24:32:30:50",
                        "count": "2"
                    },
                    {
                        "address": "12:42:32:30:50",
                        "count": "4"
                    },
                    {
                        "address": "12:42:14:88:50",
                        "count": "1"
                    }
                ]
            }

            Component
            {
                id:tableViewDelegate
                Rectangle {
                id: delegateTableItem;
                implicitWidth: 100
                implicitHeight: 50
                radius: 4;
                state: "default"

                Text {
                    text: textToDisplay
                    anchors.centerIn: parent
                }

                states:[
                    State {
                        name: "default"
                        PropertyChanges {
                            target: delegateTableItem
                            color: "#efebe9"
                        }
                    },
                    State {
                        name: "checked"
                        PropertyChanges {
                            target: delegateTableItem
                            color:"#ffab91"
                        }
                    }
                ]
            }
            }

            TableView{
                id: addressStorageTableView
                Layout.preferredWidth: appRoot.width * 0.2;
                Layout.preferredHeight: appRoot.height * 0.6;
                Layout.leftMargin: 10;
                Layout.rightMargin: 10;
                columnSpacing: 1
                rowSpacing: 1

                model: addressStorage;

                delegate:
                    DelegateChooser{

                        DelegateChoice
                        {
                            column: 0;
                            delegate: Loader
                            {
                                sourceComponent: tableViewDelegate
                                property string textToDisplay: address;
                            }
                        }
                        DelegateChoice
                        {
                            column: 1;
                            delegate: Loader
                            {
                                sourceComponent: tableViewDelegate
                                property string textToDisplay: count;
                            }
                        }
                    }
            }
        }
        Row{
            spacing: 4;
            Button
            {
                id: clearAllBt
                text: qsTr("Clear All");
                onClicked:
                {
                    addressHandler.clearAll();
                }
            }
            Button
            {
                id: addAddressBt
                text: qsTr("Add address");
                onClicked:
                {
                    console.log("Requested address add");
                    if( addressHandler.isAddressValid()){
                        let toAdd = addressHandler.getCurrentAddress();
                        addressStorage.addAddress(toAdd);
                        console.log("Address is:", toAdd);
                    }
                }
            }
            Button
            {
                id: findAddressButton
                text: qsTr("Find address");
                onClicked:
                {
                    console.log("Requested address find");
                }
            }
        }

        Row
        {
            id: addressInputs;

            Repeater{
                model: 5
                TextField {
                    id: fieldRoot;
                    width: 40
                    validator: IntValidator  {bottom: 0; top: 99;}
                    focus: true
                    onEditingFinished:
                    {
                        addressHandler.onItemEditFinished(fieldRoot.text, index)
                        console.log(fieldRoot.text, index);
                    }

                    Connections
                    {
                        target: addressHandler
                        function onClearAllSig(){
                            fieldRoot.text = "";
                        }

                        function onOnItemHiglhightSetRequested(itemId,addressPosition){
                            if(addressPosition === index){
                                fieldRoot.text = itemId;
                            }
                        }
                        function onOnItemHiglhightResetRequested(itemId,addressPosition){
                            if(addressPosition === index)
                            {
                                fieldRoot.text = "";
                            }
                        }
                    }
                }
            }
        }
    }
}
