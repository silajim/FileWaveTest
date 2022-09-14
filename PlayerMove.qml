import QtQuick
import QtQuick.Controls
import com.AppUI 1.0

Item {
    property alias playerName:playerlbl.text
    property alias canMove: combo.visible
    height: parent.height
    Column{
        anchors.centerIn: parent
        Label{
            id: playerlbl
        }
        Connections{
            target: app
//            function onMoveMade(username){
//                if(playerlbl.text===username){
//                    combo.visible = false;
//                }
//            }
            function onNextGameStarted(){
                combo.currentIndex = 0;
//                combo.visible = true
            }
        }

        ComboBox{
            id:combo
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Rock" , value: AppUI.Rock},
                {text: "Paper" , value: AppUI.Paper},
                {text: "Scissors" , value: AppUI.Scissors}
            ]
            onCurrentIndexChanged: console.log("index " + currentIndex)
            onCurrentValueChanged: console.log("Value " + currentValue)

        }
        Button{
            text: "Select"
            enabled: combo.visible
            onClicked: {
                console.log("Selected " + combo.valueAt(combo.currentIndex) )
                app.makeMove(playerlbl.text,combo.valueAt(combo.currentIndex))
            }
        }
    }

}
