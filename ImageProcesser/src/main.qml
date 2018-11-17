import QtQuick 2.9
import QtQuick.Window 2.2
import com.st.ImageProcesser 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQml 2.2

Window {
    visible: true;
    width: 640;
    height: 480;
    title: qsTr("ImageProcesser");

    Image {
        id: imageviwer;
        asynchronous: true;
        anchors.fill: parent;
        fillMode: Image.PreserveAspectFit;
    }
    ImageProcesser {
        id: imageprocesser;
        onFinished:{
            imageviwer.source = "file:///" + newFile;
        }
    }

    FileDialog {
        id: fileDialog;
        title: "打开图片";
        folder: shortcuts.desktop;
        nameFilters: ["Image files (*.jpg *.png)", "All files (*)"];
        onAccepted: {
            console.log(fileDialog.fileUrl);
            imageviwer.source = fileDialog.fileUrl;
        }
    }

    Button {
        id: openBtn;
        text: "打开"
        anchors.left: parent.left;
        anchors.leftMargin: 6;
        anchors.top: parent.top;
        anchors.topMargin: 6;
        z: 1;

        onClicked: {
            fileDialog.visible = true;
        }
    }

    Button {
        id: closeBtn;
        text: "退出";
        anchors.left: openBtn.right;
        anchors.leftMargin: 6;
        anchors.bottom: openBtn.bottom;
        z: 1;

        onClicked: {
            Qt.quit();
        }
    }

    Grid {
        id: group;
        rows: 2;
        columns: 3;
        anchors.left: parent.left;
        anchors.leftMargin: 6;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 6;
        rowSpacing: 4;
        columnSpacing: 4;
        z: 1;

        Button {
            text: "锐化";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.SHARPEN);
            }
        }

        Button {
            text: "灰度";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.GRAY);
            }
        }

        Button {
            text: "浮雕";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.EMBOSS);
            }
        }

        Button {
            text: "黑白";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.BINARIZE);
            }
        }


        Button {
            text: "底片";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.NEGATIVE);
            }
        }

        Button {
            text: "模糊";
            onClicked: {
                imageprocesser.process(fileDialog.fileUrl, ImageProcesser.SOFTEN);
            }
        }
    }

}
