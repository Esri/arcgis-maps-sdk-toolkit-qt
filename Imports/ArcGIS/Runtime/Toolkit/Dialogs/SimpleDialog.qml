import QtQuick 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.1

/*
  standardButtons:
        AbstractDialog.NoButton
        AbstractDialog.Ok
        AbstractDialog.Save
        AbstractDialog.SaveAll
        AbstractDialog.Open
        AbstractDialog.Yes
        AbstractDialog.YesToAll
        AbstractDialog.No
        AbstractDialog.NoToAll
        AbstractDialog.Abort
        AbstractDialog.Retry
        AbstractDialog.Ignore
        AbstractDialog.Close
        AbstractDialog.Cancel
        AbstractDialog.Discard
        AbstractDialog.Help
        AbstractDialog.Apply
        AbstractDialog.Reset
        AbstractDialog.RestoreDefaults

  modality:
        Qt.NonModal
        Qt.WindowModal
        Qt.ApplicationModal
*/

Dialog {
    id: dialog
    modality: Qt.ApplicationModal
    visible: false
    standardButtons: AbstractDialog.Close
}
