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
    void on_tap_clicked(); // 탭 버튼 추가

private:
    Ui::MainWindow *ui;
    bool isKoreanKeyboard;  // 한/영 키보드 전환 상태
    bool isCapsLock;        // Caps Lock 상태
    bool isShiftPressed;    // Shift 키 눌림 상태

    void insertText(const QString &text); // 텍스트 삽입 함수
    void moveCursorToEnd();  // 커서를 텍스트 끝으로 이동
    void simulateWinKeyPress(); // Win 키 이벤트 시뮬레이션

protected:
    void keyPressEvent(QKeyEvent *event) override;  // 키 입력 처리
    void keyReleaseEvent(QKeyEvent *event) override;  // 키 해제 처리
};

#endif // MAINWINDOW_H
