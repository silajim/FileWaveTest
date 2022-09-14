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


    onOpened: {
//        userdialog.focus = true
        textf.forceActiveFocus();
    }

    title: "login user"

    Shortcut{
        enabled: true
        sequences: [Qt.Key_Enter , Qt.Key_Return]
        onActivated: {
            console.log("Activated")
            userdialog.accept();
        }
    }

    TextField{
        id:textf
        anchors.centerIn: parent
        Keys.onReturnPressed: userdialog.accept(); // Enter key
        Keys.onEnterPressed: userdialog.accept(); // Numpad enter key
//        focus: true
    }

    onAccepted:{
        app.login(textf.text)
        textf.text=""
    }
    onRejected: close();
}
