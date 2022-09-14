import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
Dialog {
    id: userdialog
    height: parent.height/3
    width: parent.width/1.5

    bottomPadding: 0
    rightPadding:  0

    x:parent.width/2-width/2
    y: parent.height/2 - height/2

    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    title: "Create user"
    TextField{
        id:textf
        anchors.centerIn: parent
        focus: true
        Keys.onReturnPressed: userdialog.accept(); // Enter key
        Keys.onEnterPressed: userdialog.accept(); // Numpad enter key
    }


    onAccepted: {
        app.createUser(textf.text)
        textf.text=""
    }
    onRejected: close();

}
