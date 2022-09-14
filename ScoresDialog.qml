import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Dialog {
    anchors.centerIn: parent

    height: parent.height - 30
    width: parent.width - 30

    property alias text:lbl.text

    modal: true

    standardButtons: "Ok"

    onAccepted: close();

    Label{
        id:lbl
        anchors.fill: parent
        anchors.margins: 10
    }

}
