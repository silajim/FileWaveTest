import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Dialog {
    id: newgamedialog
    height: parent.height/3
    width: parent.width/1.5

    bottomPadding: 0
    rightPadding:  0

    x:parent.width/2-width/2
    y: parent.height/2 - height/2

    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    title: "New game"
    onOpened: {
        gamename.forceActiveFocus();
    }


    Column{
        anchors.centerIn: parent
        spacing: 5
        TextField{
            id:gamename
            placeholderText: "GameName"
            validator: RegularExpressionValidator { regularExpression: /[^\s]+/ }
            focus:true
        }
        TextField{
            id:player1name
            placeholderText: "Player 1 Username"
            validator: RegularExpressionValidator { regularExpression: /[^\s]+/ }
        }
        TextField{
            id:player2name
            placeholderText: "Player 2 Username"
            validator: RegularExpressionValidator { regularExpression: /[^\s]+/ }
            Keys.onReturnPressed: newgamedialog.accept(); // Enter key
            Keys.onEnterPressed: newgamedialog.accept(); // Numpad enter key
        }
    }

    Connections{
        target: app
        function onGameCreated(){
            close();
        }
    }

    onAccepted: {
        app.createGameSession(player1name.text,player2name.text,gamename.text)
    }
    onRejected: close();

    onClosed:{
        gamename.text=""
        player1name.text=""
        player2name.text=""
    }


}
