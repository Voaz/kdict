import QtQuick 2.6
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import org.voaz.translator 1.0
import QtQuick.Layouts 1.12

Window {
    id: root
    title: "Trnslator"
    width: 200
    height: 200
    y: 400
    visible: true
    color: "transparent"
    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    onWindowStateChanged: {
        console.log(windowState);
    }
    onActiveChanged: {

        console.log(root.active)
    }

    Column {
        anchors.fill: parent
        Rectangle {
            id: targetRect
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#66dbf7f5"
            TextEdit {
                id: target
                font.pointSize: 14
                anchors.fill: parent
                text: tr.sourceWord

            }
        }
        Rectangle {
            color: "#66cdf1ee"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            TextEdit {
                id: result
                font.pointSize: 10
                anchors.fill: parent
                text: tr.targetWord
                wrapMode: TextEdit.WordWrap
            }
        }
    }
    Rectangle {
        anchors.top: targetRect.bottom
        height: 3
        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.YAxis
            ondr
        }
    }
    Translator {
        id: tr
    }
}
