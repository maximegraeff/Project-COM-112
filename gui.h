#ifndef GUI_H
#define GUI_H

#include <array>
#include <gtkmm.h>
#include <string>

extern GameData game_data;

class My_window : public Gtk::Window
{
public:
    My_window(std::string file_name);

private:
    Gtk::Box main_box,drawing_box, panel_box, command_box;
    Gtk::Grid info_grid;
    bool loop_activated;
    sigc::connection loop_conn;
    std::array<Gtk::Button, 6> buttons;
    Gtk::Frame info_frame;
    std::array<Gtk::Label, 4> info_text, info_value;
    Gtk::DrawingArea drawing;
    std::string last_read_file;

    void set_commands();

    void exit_clicked();
    void open_clicked();
    void save_clicked();
    void restart_clicked();
    void start_clicked();
    void step_clicked();

    void set_key_controller();
    bool key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);

    void set_dialog(Gtk::FileChooserDialog *dialog);
    void dialog_response(int response, Gtk::FileChooserDialog *dialog);

    bool loop();

    void set_infos();
    void update_infos();

    void set_drawing();
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
    
    void set_mouse_controller();
    void on_drawing_left_click(int n_press, double x, double y);
    //bool new_ball_intersects(const Circle& new_ball) const;
    void on_drawing_move(double x, double y);
    void update_paddle();
    pair<double, double> limit_delta(double dx, double dy);
    double paddle_collision(double x, double temp_x, double y, double r, double dx);
    
    /*void update_balls();
    pair<double, double> ball_collision(const unique_ptr<Ball>& ball, double dx,
        double dy);
    pair<double, double> ball_bricks_collision(double x_b, double y_b, double r, 
        double dx, double dy, double x_brick, double y_brick, double w);
    pair<double, double> ball_paddle_collision(double x_b, double y_b, double r, 
        double dx, double dy);
    pair<double, double> ball_circle_collision(double r, double dx, double dy,
        double r_c, double dx_, double dy_);*/
};

#endif
