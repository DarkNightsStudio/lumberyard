/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#ifndef QKeySequenceEditorDialog_h__
#define QKeySequenceEditorDialog_h__

#include <QDialog>
#include "QKeySequenceCaptureWidget.h"
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTreeWidget>

struct HotKey;
class QKeySequenceCaptureWidget;

class KeyWidgetItem
    : public QWidget
{
    Q_OBJECT
public:
    explicit KeyWidgetItem(QWidget* parent = nullptr);
    ~KeyWidgetItem();

    void SetLabel(const char* text) { m_label->setText(text); }
    void SetLabel(QString& text) { m_label->setText(text); }
    void SetCapture(QKeySequenceCaptureWidget* captureWidget);

private:
    QGridLayout* m_gridLayout;
    QLabel* m_label;
    QKeySequenceCaptureWidget* m_captureWidget;
};

class QKeySequenceEditorDialog
    : public QDialog
{
protected:
    void BuildLayout();
    void SetHotKeysFromEditor();
    bool ValidateKeys();
    void StoreSessionState();
    void LoadSessionState();
    virtual void hideEvent(QHideEvent*) override;
private:
    friend class UIFactory;
    QKeySequenceEditorDialog(QWidget* parent);
    ~QKeySequenceEditorDialog();
public:
    bool onFieldUpdate(QKeySequenceCaptureWidget* field);
    void resetHotkeys();

protected:
    //local copy of hotkeys
    QVector<HotKey> m_hotkeys;
    QVector<QPair<QLabel*, QKeySequenceCaptureWidget*> > m_fields;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* menuButton;
    QFormLayout* formLayout;
    QScrollArea* scrollArea;
    QWidget*     scrollWidget;
    QGridLayout* layout;
    QTreeWidget* m_treeWidget;
};
#endif // QKeySequenceEditorDialog_h__
