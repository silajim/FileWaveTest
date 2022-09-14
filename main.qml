import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
ApplicationWindow  {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Dialog{
        id:errordialog
        property alias text:errorlbl.text
        standardButtons: Dialog.Ok
        modal:true
        anchors.centerIn: parent
        Label{
            id:errorlbl

        }
    }
    Connections{
        target: app
        function onErrorMessage(message){
            errordialog.text = message
            errordialog.open();
        }
        function onWinner(text){
            errordialog.text = text
            errordialog.open();
        }
    }

    CreateUserDialog{
        id:createuserdial
        visible: false
    }
    LoginUserDialog{
        id:loginuserdial
        visible: false
    }
    CreateNewGame{
        id:newgamed
    }
    ScoresDialog{
        id:scores
    }

    header: Item{
        height: createbtn.height + 10
        width: parent.width
        Row{
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5
            Button{
                id: createbtn
                text: qsTr("Create User")
                onClicked: createuserdial.open();
            }
            Button{
                text: qsTr("Login user")
                onClicked: loginuserdial.open();
            }
            Button{
                text: qsTr("New Game")
                onClicked: newgamed.open();
            }
            ComboBox{
                textRole: "display"
                model:app.gameCombomodel
                onCurrentIndexChanged: {
                    app.gameselectedChanged(currentIndex)
                }
            }

            Button{
                text: "High Scores"
                onClicked: {
                    scores.text = app.scores();
                    scores.open();
                }
            }
        }
    }

    Rectangle{
        anchors{
            top:header.bottom
            left:parent.left
            right:parent.right
            bottom:parent.bottom
        }

        Row{
            anchors.fill: parent
            PlayerMove{
                id:player1
                //            height: parent.height
                width: (parent.width/3)-15
                playerName: app.player1Name
                canMove:app.canp1move

            }
            Rectangle{
                width: (parent.width/3)-15
                height: parent.height
                Column{
                    anchors.centerIn: parent
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        text:app.gameName
                    }
                    Button{
                        text: "Next Game"
                        enabled: app.gamefinished
                        onClicked: app.startNextGame();
                    }
                }
            }
            PlayerMove{
                id:player2
                //            height: parent.height
                width: (parent.width/3)-15
                playerName: app.player2Name
                canMove:app.canp2move

            }
        }
    }

}
