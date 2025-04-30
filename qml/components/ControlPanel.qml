import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.15

Rectangle {
	id: root
	color: "white"
	property string status
	signal cancel
	signal pause
	signal play
	signal start
	RowLayout{
		id: buttonRow
		anchors.fill: parent
		anchors.leftMargin: parent.width * 0.05
		anchors.rightMargin: parent.width * 0.05
		spacing: parent.width * 0.025
		ButtonCustom{
			text: "Отмена"
			enabled: root.status == "default"? false: true
			normalColor: enabled? "#dc0d0d": "#989bab"
			impH: parent.height * 0.8
			Layout.fillWidth: true
			onClicked: {
				root.cancel()
			}
		}
		ButtonCustom{
			text: "Пауза"
			enabled: root.status == "default"? false:
											   root.status == "pause"? false: true
			normalColor: enabled? "#30ad00": "#989bab"
			impH: parent.height * 0.8
			Layout.fillWidth: true
			onClicked: {
				root.pause()
			}
		}
		ButtonCustom{
			text: "Продолжить"
			enabled: root.status == "default"? false:
											   root.status == "inprogress"? false: true
			normalColor: enabled? "#30ad00": "#989bab"
			impH: parent.height * 0.8
			Layout.fillWidth: true
			onClicked: {
				root.play()
			}
		}
		ButtonCustom{
			text: "Старт"
			enabled: root.status == "default"? true: false
			normalColor: enabled? "#30ad00": "#989bab"
			impH: parent.height * 0.8
			Layout.fillWidth: true
			onClicked: {
				root.start()
			}
		}
	}
}
