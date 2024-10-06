#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QTextCursor>
#include <QKeyEvent>

// X11 헤더 파일 포함 전에 None 정의 해제
#undef None
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isKoreanKeyboard(false)
    , isCapsLock(false)
    , isShiftPressed(false)
{
    ui->setupUi(this);
    // 키 버튼 클릭 이벤트 설정
    for (int i = 1; i <= 47; ++i) {
        QPushButton *button = this->findChild<QPushButton*>(QString("key_%1").arg(i));
        if (button) {
            connect(button, &QPushButton::clicked, this, &MainWindow::on_key_clicked);
        }
    }
    connect(ui->copy, &QPushButton::clicked, this, &MainWindow::on_copy_clicked);
    connect(ui->clear, &QPushButton::clicked, this, &MainWindow::on_clear_clicked);
    connect(ui->space, &QPushButton::clicked, this, &MainWindow::on_space_clicked);
    connect(ui->enter, &QPushButton::clicked, this, &MainWindow::on_enter_clicked);
    connect(ui->backspace, &QPushButton::clicked, this, &MainWindow::on_backspace_clicked);
    connect(ui->capslock, &QPushButton::clicked, this, &MainWindow::on_capsLock_clicked);
    connect(ui->win_l, &QPushButton::clicked, this, &MainWindow::on_win_l_clicked);
    connect(ui->win_r, &QPushButton::clicked, this, &MainWindow::on_win_r_clicked);
    connect(ui->shift_l, &QPushButton::clicked, this, &MainWindow::on_shift_l_clicked);
    connect(ui->shift_r, &QPushButton::clicked, this, &MainWindow::on_shift_r_clicked);
    connect(ui->tap, &QPushButton::clicked, this, &MainWindow::on_tap_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::simulateWinKeyPress() {
    Display *display = XOpenDisplay(nullptr);
    if (!display) {
        qDebug() << "Unable to open X display";
        return;
    }
    // 'Super_L' 키 이벤트를 시뮬레이션
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XStringToKeysym("Super_L")), True, 0);
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XStringToKeysym("Super_L")), False, 0);
    XFlush(display);
    XCloseDisplay(display);
}

void MainWindow::on_win_l_clicked() {
    simulateWinKeyPress();
    qDebug() << "Left Windows key pressed.";
}

void MainWindow::on_win_r_clicked() {
    simulateWinKeyPress();
    qDebug() << "Right Windows key pressed.";
}

void MainWindow::on_kor_clicked() {
    if (isKoreanKeyboard) {
        QString engKeys[47] = {
            "`\n!", "1\n!", "2\n@", "3\n#", "4\n$", "5\n%", "6\n^", "7\n&", "8\n*", "9\n(", "0\n)", "-\n_", "=\n+",
            "q\nQ", "w\nW", "e\nE", "r\nR", "t\nT", "y\nY", "u\nU", "i\nI", "o\nO", "p\nP", "[\n{", "]\n}", "₩\n|",
            "a\nA", "s\nS", "d\nD", "f\nF", "g\nG", "h\nH", "j\nJ", "k\nK", "l\nL", ";\n:", "'\n'",
            "z\nZ", "x\nX", "c\nC", "v\nV", "b\nB", "n\nN", "m\nM", ",\n<", ".\n>", "/\n?"
        };
        for (int i = 0; i < 47; ++i) {
            QPushButton* button = this->findChild<QPushButton*>(QString("key_%1").arg(i + 1));
            if (button) {
                button->setText(engKeys[i]);
            }
        }
        isKoreanKeyboard = false; // 현재 상태를 영어로 변경
    } else {
        QString korKeys[47] = {
            "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
            "ㅂ", "ㅈ", "ㄷ", "ㄱ", "ㅅ", "ㅛ", "ㅕ", "ㅑ", "ㅐ", "ㅔ", "[", "]", "₩",
            "ㅁ", "ㄴ", "ㅇ", "ㄹ", "ㅎ", "ㅗ", "ㅓ", "ㅏ", "ㅣ", ";", "'",
            "ㅋ", "ㅌ", "ㅊ", "ㅍ", "ㅠ", "ㅜ", "ㅡ", ",", ".", "/"
        };
        for (int i = 0; i < 47; ++i) {
            QPushButton* button = this->findChild<QPushButton*>(QString("key_%1").arg(i + 1));
            if (button) {
                button->setText(korKeys[i]);
            }
        }
        isKoreanKeyboard = true; // 현재 상태를 한글로 변경
    }
}

void MainWindow::on_key_clicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString keyText = button->text();

        // 특수 기호 처리
        if (keyText.contains("\n")) {
            keyText = keyText.split("\n").first(); // 텍스트가 두 줄인 경우 첫 번째 줄만 사용
        }

        // CapsLock이 활성화된 상태에서 한글일 경우 쌍자음 처리
        if (isKoreanKeyboard) {
            if (isShiftPressed) {
                if (keyText == "ㅂ") keyText = "ㅃ";
                else if (keyText == "ㅈ") keyText = "ㅉ";
                else if (keyText == "ㄷ") keyText = "ㄸ";
                else if (keyText == "ㄱ") keyText = "ㄲ";
                else if (keyText == "ㅅ") keyText = "ㅆ";
                else if (keyText == "ㅐ") keyText = "ㅒ";
                else if (keyText == "ㅔ") keyText = "ㅖ";
            }
        } else if (!isKoreanKeyboard) {
            if (isCapsLock || isShiftPressed) {
                keyText = keyText.toUpper();
            } else {
                keyText = keyText.toLower();
            }
        }

        insertText(keyText); // 입력된 키를 텍스트에 추가
        moveCursorToEnd(); // 커서를 끝으로 이동
    }
}

void MainWindow::insertText(const QString &text) {
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertText(text);
    ui->textEdit->setTextCursor(cursor);
    // 커서 위치를 조정
    moveCursorToEnd();
}

void MainWindow::moveCursorToEnd() {
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::on_copy_clicked() {
    QString allText = ui->textEdit->toPlainText(); // 텍스트 에디터의 모든 텍스트 가져오기
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(allText); // 클립보드에 텍스트 설정
}

void MainWindow::on_clear_clicked() {
    ui->textEdit->clear(); // 텍스트 에디터 비우기
    moveCursorToEnd(); // 커서를 마지막 위치로 이동
}

void MainWindow::on_space_clicked() {
    insertText(" "); // 공백 삽입
}

void MainWindow::on_enter_clicked() {
    insertText("\n"); // 줄바꿈 삽입
}

void MainWindow::on_backspace_clicked() {
    QTextCursor cursor = ui->textEdit->textCursor();

    if (cursor.hasSelection()) {
        cursor.removeSelectedText(); // 선택된 텍스트가 있으면 삭제
    } else {
        cursor.deletePreviousChar(); // 선택된 텍스트가 없으면 이전 문자를 삭제
    }
    ui->textEdit->setTextCursor(cursor); // 커서 위치 업데이트
}

void MainWindow::on_capsLock_clicked() {
    isCapsLock = !isCapsLock; // Caps Lock 토글
    ui->capslock->setStyleSheet(isCapsLock ? "background-color: darkgray;" : ""); // 버튼 색상 변경
}

void MainWindow::on_shift_l_clicked() {
    isShiftPressed = true; // Shift 키 눌림 처리
}

void MainWindow::on_shift_r_clicked() {
    isShiftPressed = true; // Shift 키 눌림 처리
}

// 탭 버튼 클릭 처리 함수
void MainWindow::on_tap_clicked() {
    QString text = ui->textEdit->toPlainText();  // 텍스트 에디터에 있는 내용을 가져옴
    qDebug() << "Current text: " << text;  // 텍스트를 디버그 출력
}

// 키 입력 처리 함수
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        isShiftPressed = true;  // Shift 키 눌림 상태로 변경
    }
    QMainWindow::keyPressEvent(event);  // 기본 키 입력 처리 호출
}

// 키 해제 처리 함수
void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Shift) {
        isShiftPressed = false;  // Shift 키 해제 상태로 변경
    }
    QMainWindow::keyReleaseEvent(event);  // 기본 키 해제 처리 호출
}
