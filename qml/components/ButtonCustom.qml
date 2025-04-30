import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtQuick.Controls.impl 2.12

Button{
	id: root

	property string textColor: "white"
	property string pressedColor: "#0168f9"//синий #0168f9 зеленый #30ad00 серый #989bab
	property string normalColor: "#30ad00"
	property int impH: 0
	property int impW: 0
	property int fontWeight: 425
	property int fontSize: 14

	background: Rectangle
	{
		implicitHeight: impH
		implicitWidth: impW
		color: root.pressed ? pressedColor :
			   root.hovered ? normalColor : normalColor
		radius: 5
	}

	contentItem: Text
	{
		text: root.text
		color: textColor
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.pixelSize: fontSize
	}

}
