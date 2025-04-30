import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
	id: root
	property var values: []
	property color barColor: "#4285F4"
	property color backgroundColor: "#e0e0e0"
	property color textColor: "white"
	property color outlineColor: "black"
	property int maxVisibleItems: 15
	property int barHeight: 24
	property int barRadius: 4
	property int textLeftMargin: 10
	property int barSpacing: 8
	property int animationDuration: 400

	readonly property real maxValue: {
		if (values.length === 0) return 1
		return Math.max(...values.map(item => item.count)) || 1
	}

	property font customFont: Qt.font({
		family: "Arial",
		pixelSize: 14,
		weight: Font.Bold
	})

	signal itemClicked(var itemData)

	Flickable {
		id: flickable
		anchors.fill: parent
		contentWidth: column.width
		contentHeight: column.height
		clip: true
		boundsBehavior: Flickable.StopAtBounds

		Column {
			id: column
			width: flickable.width
			spacing: barSpacing

			Repeater {
				model: values.slice(0, maxVisibleItems)

				Item {
					width: parent.width
					height: barHeight + 4

					Rectangle {
						id: backgroundBar
						width: parent.width
						height: barHeight
						color: backgroundColor
						radius: barRadius
						anchors.verticalCenter: parent.verticalCenter
					}
					Rectangle {
						id: valueBar
						width: Math.min(
							(modelData.count / root.maxValue) * (parent.width - wordLabel.width - countLabel.width - 30),
							parent.width - wordLabel.width - countLabel.width - 30
						)
						height: barHeight
						color: barColor
						radius: barRadius
						anchors.verticalCenter: parent.verticalCenter

						Behavior on width {
							NumberAnimation {
								duration: animationDuration
								easing.type: Easing.OutCubic
							}
						}
					}
					Text {
						id: wordLabel
						text: modelData.word
						color: textColor
						font: customFont
						style: Text.Outline
						styleColor: outlineColor
						width: 120
						elide: Text.ElideRight
						anchors {
							left: parent.left
							leftMargin: textLeftMargin
							verticalCenter: parent.verticalCenter
						}
					}

					Text {
						id: countLabel
						text: modelData.count
						color: textColor
						font: customFont
						style: Text.Outline
						styleColor: outlineColor
						anchors {
							right: parent.right
							rightMargin: 10
							verticalCenter: parent.verticalCenter
						}
					}

					MouseArea {
						anchors.fill: parent
						hoverEnabled: true
						onClicked: root.itemClicked(modelData)
						onEntered: valueBar.opacity = 0.8
						onExited: valueBar.opacity = 1
					}
				}
			}
		}
	}

	Text {
		anchors.centerIn: parent
		text: "Нет данных для отображения"
		color: "gray"
		font.pixelSize: 16
		visible: values.length === 0
	}
}
