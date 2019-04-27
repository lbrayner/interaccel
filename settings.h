#ifndef SETTINGS_H

class Settings
{
    double sensitivity_;
    double acceleration_;
    double offset_;
    double power_;
    double pre_scale_x_;
    double pre_scale_y_;
    double post_scale_x_;
    double post_scale_y_;
    double angle_adjustment_;
    double angle_snapping_;
    double speed_cap_;
    bool fancy_output_;
public:
    Settings();
    Settings(
        double sensitivity,
        double acceleration,
        double offset,
        double power,
        double pre_scale_x,
        double pre_scale_y,
        double post_scale_x,
        double post_scale_y,
        double angle_adjustment,
        double angle_snapping,
        double speed_cap,
        bool fancy_output
        );
    ~Settings();
    double const sensitivity() const { return sensitivity_; }
    double const acceleration() const { return acceleration_; }
    double const offset() const { return offset_; }
    double const power() const { return power_; }
    double const pre_scale_x() const { return pre_scale_x_; }
    double const pre_scale_y() const { return pre_scale_y_; }
    double const post_scale_x() const { return post_scale_x_; }
    double const post_scale_y() const { return post_scale_y_; }
    double const angle_adjustment() const { return angle_adjustment_; }
    double const angle_snapping() const { return angle_snapping_; }
    double const speed_cap() const { return speed_cap_; }
    bool const fancy_output() const { return fancy_output_; }
};

#endif // SETTINGS_H
