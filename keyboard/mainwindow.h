#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_win_l_clicked();
    void on_win_r_clicked();
    void on_kor_clicked();
    void on_key_clicked();
    void on_copy_clicked();
    void on_clear_clicked();
    void on_space_clicked();
    void on_enter_clicked();
    void on_backspace_clicked();
    void on_capsLock_clicked();
    void on_shift_l_clicked();
    void on_shift_r_clicked();
    void on_tap_clicked(); // 이 줄을 추가하세요.

private:
    Ui::MainWindow *ui;
    bool isKoreanKeyboard;
    bool isCapsLock;
    bool isShiftPressed;

    void insertText(const QString &text);
    void moveCursorToEnd();
    void simulateWinKeyPress(); // Win 키 이벤트 시뮬레이션 함수 선언

protected:
    void keyPressEvent(QKeyEvent *event) override; // 키 입력 처리
    void keyReleaseEvent(QKeyEvent *event) override; // 키 해제 처리

};

#endif // MAINWINDOW_H
