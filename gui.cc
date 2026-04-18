#include <filesystem>
#include <iostream>
#include <algorithm>
#include <tuple>
#include "constants.h"
#include "game.h"
#include "graphic_gui.h"
#include "gui.h"
#include "game.h"
#include "graphic.h"

using namespace std;

enum Response
{
    CANCEL,
    OPEN_FILE,
    SAVE_FILE
};
enum Buttons
{
    EXIT,
    OPEN,
    SAVE,
    RESTART,
    START,
    STEP
};

constexpr unsigned drawing_size(500);

My_window::My_window(string file_name)
    : main_box(Gtk::Orientation::HORIZONTAL),drawing_box(Gtk::Orientation::HORIZONTAL), panel_box(Gtk::Orientation::VERTICAL),
      command_box(Gtk::Orientation::VERTICAL), loop_activated(false),
      buttons({Gtk::Button("exit"), Gtk::Button("open"), Gtk::Button("save"),
               Gtk::Button("restart"), Gtk::Button("start"), Gtk::Button("step")}),
      info_frame("Infos :"), info_text({Gtk::Label("score:"), Gtk::Label("lives:"),
                                        Gtk::Label("bricks:"), Gtk::Label("balls:")}),
      last_read_file(file_name)
{

    set_title("Brick Breaker");
    set_child(main_box);
    main_box.append(panel_box);
    main_box.append(drawing_box);
    drawing_box.append(drawing);
    panel_box.append(command_box);
    panel_box.append(info_frame);

    set_commands();
    set_key_controller();
    set_mouse_controller();
    set_infos();
    set_drawing();

    if (!last_read_file.empty())
    {
        read(last_read_file);
        update_infos();
    }

    // Remplace le bloc CSS actuel par ceci :
    drawing_box.set_name("drawing-box");

    auto css = Gtk::CssProvider::create();
    css->load_from_data("#drawing-box { background-color: #888888; }");
    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(), css,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

}
void My_window::set_commands()
{
    for (auto &button : buttons)
    {
        command_box.append(button);
        command_box.set_size_request(120, -1);
        button.set_margin(1);
    }

    buttons[EXIT].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::exit_clicked));
    buttons[OPEN].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::open_clicked));
    buttons[SAVE].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::save_clicked));
    buttons[RESTART].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::restart_clicked));
    buttons[START].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::start_clicked));
    buttons[STEP].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::step_clicked));
}

void My_window::exit_clicked()
{
    hide();
}
void My_window::open_clicked()
{
    cout.flush();
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::OPEN);
    dialog->set_transient_for(*this); 
    set_dialog(dialog);
}
void My_window::save_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::SAVE);
    set_dialog(dialog);
}
void My_window::restart_clicked()
{
    cout << __func__ << endl;
    if (!last_read_file.empty())
    {
        read(last_read_file);
        update_infos();
        drawing.queue_draw();
    }
}
void My_window::start_clicked()
{
    cout << __func__ << endl;
    if (loop_activated)
    {
        loop_conn.disconnect();
        loop_activated = false;
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[STEP].set_sensitive(true);
    }
    else // TODO: only if the game is not finished
    {
        loop_conn =
            Glib::signal_timeout().connect(sigc::mem_fun(*this, &My_window::loop), dt);
        loop_activated = true;
        buttons[EXIT].set_sensitive(false);
        buttons[OPEN].set_sensitive(false);
        buttons[SAVE].set_sensitive(false);
        buttons[RESTART].set_sensitive(false);
        buttons[START].set_label("stop");
        buttons[STEP].set_sensitive(false);
    }
}
void My_window::step_clicked()
{
    loop_activated = true;
    loop_activated = !loop();
    cout << __func__ << endl;
}
void My_window::set_key_controller()
{
    auto contr = Gtk::EventControllerKey::create();
    contr->signal_key_pressed().connect(sigc::mem_fun(*this, &My_window::key_pressed),
                                        false);
    add_controller(contr);
}
bool My_window::key_pressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
    switch (keyval)
    {
    case '1':
        if (!loop_activated) step_clicked();
        return true;
    case 's':
        start_clicked();
        return true;
    case 'r':
        restart_clicked();
        return true;
    default:
        break;
    }
    return false;
}

void My_window::set_dialog(Gtk::FileChooserDialog *dialog)
{
    dialog->set_modal(true);
    dialog->set_transient_for(*this);
    dialog->set_select_multiple(false);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &My_window::dialog_response), dialog));
    

    dialog->add_button("_Cancel", CANCEL);
    switch (dialog->get_action())
    {
    case Gtk::FileChooserDialog::Action::OPEN:
        dialog->add_button("_Open", OPEN_FILE);
        break;
    case Gtk::FileChooserDialog::Action::SAVE:
        dialog->add_button("_Save", SAVE_FILE);
        break;
    default:
        break;
    }

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_pattern("*.txt");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->set_hide_on_close(false);
    dialog->show();
}
void My_window::dialog_response(int response, Gtk::FileChooserDialog *dialog)
{
    filesystem::path file_name = "";
    if (dialog->get_file())
    {
        file_name = dialog->get_file()->get_path();
        if (file_name.extension() != ".txt")
        {
            file_name = "";
        }
    }
    switch (response)
    {
    case CANCEL:
        dialog->hide();
        break;
    case OPEN_FILE:
        if (file_name != "")
        {
            cout << "open file " << file_name << endl;
            last_read_file = file_name.string();
            read(file_name.string());
            update_infos();
            drawing.queue_draw();
            dialog->hide();
        }
        break;
    case SAVE_FILE:
        {
            string save_path = "";
            if (dialog->get_file()){
                save_path = dialog->get_file()->get_path();
                if (filesystem::path(save_path).extension() != ".txt"){
                    save_path += ".txt";
                }
            }
            if (!save_path.empty()){
                cout << "save file " << save_path << endl;
                save_game(game_data, save_path);
                drawing.queue_draw();
                dialog->hide();
            }
            break;
        }
    default:
        break;
    }
}

bool My_window::loop()
{
    if (loop_activated)
    {   
        update_balls();
        update_paddle();
        update_infos();
        drawing.queue_draw();
        return true;
    }
    return false;
}

void My_window::set_infos()
{
    info_frame.set_child(info_grid);
    info_grid.set_column_homogeneous(true);
    for (size_t i(0); i < info_text.size(); ++i)
    {
        info_grid.attach(info_text[i], 0, i, 1, 1);
        info_grid.attach(info_value[i], 1, i, 1, 1);
        info_text[i].set_halign(Gtk::Align::START);
        info_value[i].set_halign(Gtk::Align::END);
        info_text[i].set_margin(3);
        info_value[i].set_margin(3);
    }
    update_infos();
}

void My_window::update_infos()

{
    info_value[0].set_text(to_string(game_data.score));
    info_value[1].set_text(to_string(game_data.lives));
    info_value[2].set_text(to_string(game_data.nb_brick));
    info_value[3].set_text(to_string(game_data.nb_ball));
}

void My_window::set_drawing()
{
    drawing.set_content_width(drawing_size);
    drawing.set_content_height(drawing_size);
    drawing.set_expand();
    drawing.set_draw_func(sigc::mem_fun(*this, &My_window::on_draw));
    drawing.set_margin(5);


}

void My_window::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    graphic_set_context(cr);
    double side(min(width, height));
    cr->translate((width - side) / 2, (height + side) / 2);
    cr->scale(side / (arena_size), -side / (arena_size));
    draw_arena();
    for (const auto& brick : game_data.bricks) brick->draw_brick();
    for (const auto& ball : game_data.balls) ball->draw_ball();
    if (game_data.paddle) game_data.paddle->draw_paddle();
}

void My_window::set_mouse_controller()
{
    auto left_click = Gtk::GestureClick::create();
    auto move = Gtk::EventControllerMotion::create();

    left_click->set_button(GDK_BUTTON_PRIMARY);

    left_click->signal_pressed().connect(
        sigc::mem_fun(*this, &My_window::on_drawing_left_click));
    move->signal_motion().connect(sigc::mem_fun(*this, &My_window::on_drawing_move));

    drawing.add_controller(left_click);
    drawing.add_controller(move);
}
void My_window::on_drawing_left_click(int n_press, double x, double y)
{
    //cout << __func__ << endl; // TODO
    if (game_data.lives > 0) {
        double x_p = game_data.paddle->getCenter_paddle().first;
        double y_p = game_data.paddle->getCenter_paddle().second;
        double r_p = game_data.paddle->getCircle().getRadius();
        Circle new_ball(x_p, y_p + r_p + new_ball_radius, new_ball_radius);
        if (!new_ball_intersects(new_ball)){
            game_data.balls.push_back(make_unique<Ball>(x_p, y_p + r_p + 
                          new_ball_radius, new_ball_radius, 0.0, new_ball_delta_norm));
            game_data.nb_ball++;
            game_data.lives--;
            update_infos();
            drawing.queue_draw();
        }
    }
}

bool My_window::new_ball_intersects(const Circle& new_ball) const {
    for (const auto& brick : game_data.bricks) {
        if (brick and intersects(new_ball, brick->getRectangle())) {
            return true;
        }
    }
    for (const auto& ball : game_data.balls) {
        if (ball and intersects(new_ball, ball->getCircle())) {
            return true;
        }
    }
    return false;
}

void My_window::on_drawing_move(double x_, double y_)
{   
    if (game_data.paddle) {
        int width = drawing.get_width();
        int height = drawing.get_height();
        double side = std::min(width, height);
        
        game_data.paddle->set_target_x((x_ - (width - side)/2) * arena_size/side);
    }
}

void My_window::update_paddle()
{   
    if (game_data.paddle) {
        int width = drawing.get_width();
        int height = drawing.get_height();
        double side = std::min(width, height);
        double x_t = game_data.paddle->get_target_x();
        double x = game_data.paddle->getCenter_paddle().first;
        double y = game_data.paddle->getCenter_paddle().second;
        double w = game_data.paddle->getWidth();
        double r = game_data.paddle->getCircle().getRadius();
    
        x_t = max(w, min(arena_size - w, x_t));

        double dx = x_t - x;
        if (abs(dx) > delta_norm_max) {
            dx = delta_norm_max*dx/abs(dx);
        }
        double temp_x = x + dx;
        // x le plus proche possible de x_t sans collision avec les briques
        double new_x = paddle_collision(x, temp_x, y, r, dx);

        if (new_x != x) {
            game_data.paddle->setCentrePaddle(new_x, y);
            drawing.queue_draw();
        }
    }
}

double My_window::paddle_collision(double x, double temp_x, double y, double r, 
                                   double dx) {
    
    Circle new_paddle_circle(temp_x, y, r);
    bool collision = false;
    for (const auto& brick : game_data.bricks) {
        if (brick and intersects(new_paddle_circle, brick->getRectangle())) {
            collision = true;
            break;
        }
    }

    if (collision) {
        double step = 0;
        if (dx > 0) step = 0.1;
        else step = -0.1;
        double best_x = x;
        double test_x = x + step;
        while (abs(test_x - x) <= abs(dx)) {
            Circle test_circle(test_x, y, r);
            bool hit = false;
            for (const auto& brick : game_data.bricks) {
                if (brick and intersects(test_circle, brick->getRectangle())) {
                    hit = true;
                    break;
                }
            }
            if (hit) break;
            best_x = test_x;
            test_x += step;
        }
        return best_x;
    }
    else {
        return temp_x;
    }
}

void My_window::update_balls() {
    if (game_data.balls.empty()) return;
    auto ball = game_data.balls.begin();
    while (ball != game_data.balls.end()) {
        if (*ball) {
            double dx = (*ball)->getDeltaVector().first;
            double dy = (*ball)->getDeltaVector().second;
            double x_b  = (*ball)->getCentre_ball().first;
            double y_b  = (*ball)->getCentre_ball().second;                
            double r  = (*ball)->getCircle().getRadius();
            bool change = true;

            while ((*ball)->bounce() and change) {
                double dx_o = dx;
                double dy_o = dy;

                // Rebond sur les bricks, le paddle et les autres balls
                tie (dx, dy) = ball_collision(*ball, dx, dy);
                
                // Rebond sur les murs gauche/droite
                if (x_b + dx < r or x_b + dx > arena_size - r) {
                    if ((*ball)->bounce()) {
                        dx = -dx;
                        (*ball)->add_bounce();
                    }
                }
                // Rebond sur le mur du haut
                if (y_b + dy > arena_size - r) {
                    if ((*ball)->bounce()) {
                        dy = -dy;
                        (*ball)->add_bounce();
                    }
                }
                if (dx == dx_o and dy == dy_o) change = false;
                else change = true;
            }
            (*ball)->reset_bounces();
            // Destruction de la ball si sortie par le bas
            if (y_b + dy < 0) {
                ball = game_data.balls.erase(ball);
                game_data.nb_ball--;
                update_infos();
            }
            // Mise à jour du delta et de la position de la ball
            else {
                (*ball)->setDeltaVector(dx, dy);
                ++ball;
            }
        }
    }
    for (const auto& ball : game_data.balls) {
        ball->update_delta();
        ball->update_position();
    }
}

// Limite la norme du vecteur (dx, dy) à delta_norm_max
pair<double, double> My_window::limit_delta(double dx, double dy) {
    if (abs(dx) > delta_norm_max) dx = delta_norm_max*dx/abs(dx);
    if (abs(dy) > delta_norm_max) dy = delta_norm_max*dy/abs(dy);
    return {dx, dy};
}

pair<double, double> My_window::ball_collision(const unique_ptr<Ball>& ball, double dx,
                                double dy) {
    double x_b = ball->getCentre_ball().first;
    double y_b = ball->getCentre_ball().second;
    double r = ball->getCircle().getRadius();
    double dx_o = dx;
    double dy_o = dy;

    for (const auto& brick : game_data.bricks) {
        if (brick) {
            double x_brick = brick->getRectangle().getCentre().first;
            double y_brick = brick->getRectangle().getCentre().second;
            double w = brick->getRectangle().getWidth();
            if (intersects(Circle(x_b + dx, y_b + dy, r), brick->getRectangle())) {
                if (ball->bounce()) {
                    ball->add_bounce();
                    return ball_bricks_collision(x_b, y_b, r, dx, dy, x_brick, y_brick,
                                                 w);
                }
                return {dx_o, dy_o};
            }  
        }
    }
    for (const auto& ball_ : game_data.balls) {
        if (ball_ and ball_ != ball) {
            double x_b_c = ball_->getCentre_ball().first;
            double y_b_c = ball_->getCentre_ball().second;
            double r_c = ball_->getCircle().getRadius();
            double dx_c = ball_->getDeltaVector().first;
            double dy_c = ball_->getDeltaVector().second;
            if (intersects(Circle(x_b + dx, y_b + dy, r), Circle(x_b_c + dx_c,
                y_b_c + dy_c, r_c))) {
                if (ball->bounce()) {
                    ball->add_bounce();
                    return ball_circle_collision(r, dx, dy, r_c, dx_c, dy_c);
                }
                return {dx_o, dy_o};
            }
        }
    }
    double x_p = game_data.paddle->getCenter_paddle().first;
    double y_p = game_data.paddle->getCenter_paddle().second;
    double r_p = game_data.paddle->getCircle().getRadius();
    double dx_p = game_data.paddle->getLast_delta();
    if (intersects(Circle(x_b + dx, y_b + dy, r), Circle(x_p + dx_p, y_p, r_p))) {
        if (ball->bounce()) {
            ball->add_bounce();
            return ball_paddle_collision(x_b, y_b, r, dx, dy);
        }
        return {dx_o, dy_o};
    }
    return {dx_o, dy_o};
}

pair<double, double> My_window::ball_bricks_collision(double x_b, double y_b, double r, // c'est chelou ce truc de con
                                double dx, double dy, double x_brick, double y_brick, 
                                double w) {        
    //double w_rebond = (abs(x_b - x_brick) < w/2) ? x_b - x_brick : (x_b < x_brick) ? 
    //                  -w/2 : w/2;
    //double l_rebond = (abs(y_b - y_brick) < w/2) ? y_b - y_brick : (y_b < y_brick) ?
    //                  -w/2 : w/2;
    
    //double dx_ = dx - 2*(x_b - x_brick + w_rebond)/r;
    //double dy_ = dy - 2*(y_b - y_brick + l_rebond)/r;

    //return limit_delta(dx_, dy_);

    // Détermine si la collision est sur un côté horizontal ou vertical
    double overlap_x = w/2 + r - abs(x_b - x_brick);
    double overlap_y = w/2 + r - abs(y_b - y_brick);

    if (overlap_x < overlap_y) {
        // Collision sur un côté gauche ou droit -> inverser dx
        dx = -dx;
    } else if (overlap_y < overlap_x) {
        // Collision sur le dessus ou dessous -> inverser dy
        dy = -dy;
    } else {
        // Collision sur un coin -> inverser les deux
        dx = -dx;
        dy = -dy;
    }

    return limit_delta(dx, dy);
}  

pair<double, double> My_window::ball_paddle_collision(double x_b, double y_b, double r,
                                double dx, double dy) {
    double x_p = game_data.paddle->getCenter_paddle().first;
    double y_p = game_data.paddle->getCenter_paddle().second;
    double r_p = game_data.paddle->getCircle().getRadius();
    double dx_p = game_data.paddle->getLast_delta();
    
    double dx_ = dx + 2*(dx_p - dx);
    double dy_ = -dy;

    return {dx_, dy_};
}

pair<double, double> My_window::ball_circle_collision(double r, double dx, double dy, 
                                double r_c, double dx_c, double dy_c) {
    double coeff = 2*pow(r_c,2)/(pow(r,2) + pow(r_c,2));

    double dx_ = dx + coeff*(dx_c - dx);
    double dy_ = dy + coeff*(dy_c - dy);

    return limit_delta(dx_, dy_);
}
 