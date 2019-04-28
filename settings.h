#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    double const sensitivity;
    double const acceleration;
    double const sensitivity_cap;
    double const offset;
    double const power;
    double const pre_scale_x;
    double const pre_scale_y;
    double const post_scale_x;
    double const post_scale_y;
    double const angle_adjustment;
    double const angle_snapping;
    double const speed_cap;
    bool const fancy_output;
    Settings(
        double const sensitivity,
        double const acceleration,
        double const sensitivity_cap,
        double const offset,
        double const power,
        double const pre_scale_x,
        double const pre_scale_y,
        double const post_scale_x,
        double const post_scale_y,
        double const angle_adjustment,
        double const angle_snapping,
        double const speed_cap,
        bool const fancy_output
    ) :
     sensitivity(sensitivity),
     acceleration(acceleration),
     sensitivity_cap(sensitivity_cap),
     offset(offset),
     power(power),
     pre_scale_x(pre_scale_x),
     pre_scale_y(pre_scale_y),
     post_scale_x(post_scale_x),
     post_scale_y(post_scale_y),
     angle_adjustment(angle_adjustment),
     angle_snapping(angle_snapping),
     speed_cap(speed_cap),
     fancy_output(fancy_output)
    {}
    class Builder;
};

class Settings::Builder
{
    double sensitivity_ = 1.0;
    double acceleration_ = 0.0;
    double sensitivity_cap_ = 0.0;
    double offset_ = 0.0;
    double power_ = 2.0;
    double pre_scale_x_ = 1.0;
    double pre_scale_y_ = 1.0;
    double post_scale_x_ = 1.0;
    double post_scale_y_ = 1.0;
    double angle_adjustment_ = 0.0;
    double angle_snapping_ = 0.0;
    double speed_cap_ = 0.0;
    bool fancy_output_ = false;
public:
    Settings const build() const {
        return
            Settings(
                sensitivity_,
                acceleration_,
                sensitivity_cap_,
                offset_,
                power_,
                pre_scale_x_,
                pre_scale_y_,
                post_scale_x_,
                post_scale_y_,
                angle_adjustment_,
                angle_snapping_,
                speed_cap_,
                fancy_output_
                );
    }
    Settings const * const hbuild() const {
        return
            new Settings(
                sensitivity_,
                acceleration_,
                sensitivity_cap_,
                offset_,
                power_,
                pre_scale_x_,
                pre_scale_y_,
                post_scale_x_,
                post_scale_y_,
                angle_adjustment_,
                angle_snapping_,
                speed_cap_,
                fancy_output_
                );
    }
    Builder& sensitivity(double const value) { sensitivity_ = value; return *this; } 
    Builder& acceleration(double const value) { acceleration_ = value; return *this; } 
    Builder& sensitivity_cap(double const value) { sensitivity_cap_ = value; return *this; } 
    Builder& offset(double const value) { offset_ = value; return *this; } 
    Builder& power(double const value) { power_ = value; return *this; } 
    Builder& pre_scale_x(double const value) { pre_scale_x_ = value; return *this; } 
    Builder& pre_scale_y(double const value) { pre_scale_y_ = value; return *this; } 
    Builder& post_scale_x(double const value) { post_scale_x_ = value; return *this; } 
    Builder& post_scale_y(double const value) { post_scale_y_ = value; return *this; } 
    Builder& angle_adjustment(double const value) { angle_adjustment_ = value; return *this; } 
    Builder& angle_snapping(double const value) { angle_snapping_ = value; return *this; } 
    Builder& speed_cap(double const value) { speed_cap_ = value; return *this; } 
    Builder& fancy_output(bool const value) { fancy_output_ = value; return *this; } 
};

#endif // SETTINGS_H
