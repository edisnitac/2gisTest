import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ProgressBar {
	id: root
	style: ProgressBarStyle {
		background: Rectangle {
			radius: 2
			color: "lightgray"
			border.color: "gray"
			border.width: 1
			implicitWidth: 200
			implicitHeight: 24
		}
		progress: Rectangle {
			color: "lightsteelblue"
			border.color: "steelblue"
		}
	}
}
