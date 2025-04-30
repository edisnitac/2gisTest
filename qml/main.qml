import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import WordStats 1.0
import "components"

Window {
	width: 640
	height: 480
	visible: true
	title: qsTr("WORDS")
	Item{
		id: root
		property string status: "default"
		anchors.fill: parent
		WordStats{
			id: ws
			onTopWordsChanged: {
				histogram.values = topWords
			}
			onProgressChanged: {
				progressBar.value = progress
			}
			onStateChanged: {
				var u = state
				root.status = u==1? "inprogress":
									u==2? "pause": "default"
			}
		}

		FileDialog{
			id: fd
			title: "Выберите текстовый файл"
			nameFilters: [
				"Текстовые файлы (*.txt *.log *.md *.csv *.json *.xml *.yml *.yaml *.ini *.conf *.cfg *.html *.css *.js *.ts *.py *.java *.kt *.sh *.bash *.ps1 *.cpp *.c *.h *.php *.go *.rs *.lua *.rb *.pl *.sql *.proto)"
			]
			onAccepted: {
				ws.openFile(fileUrl)
			}
		}
		HistogramCustom{
			id: histogram
			anchors.top: parent.top
			anchors.left: parent.left
			width: parent.width
			height: parent.height * 0.8
		}
		ProgressBarCustom{
			id: progressBar
			value: ws.progress
			Behavior on value {
				NumberAnimation {
					duration: 100 // Плавная анимация
					easing.type: Easing.Linear
				}
			}
			anchors.top: histogram.bottom
			anchors.left: parent.left
			width: parent.width
			height: parent.height * 0.05
		}
		ControlPanel{
			id: controlPanel
			status: root.status
			anchors.top: progressBar.bottom
			anchors.left: parent.left
			width: parent.width
			height: parent.height * 0.15
			onStart: fd.open()

			onPause: ws.pauseProcessing()
			onPlay: ws.resumeProcessing()
			onCancel: ws.cancelProcessing()
		}
	}
}
