#ifndef SETTINGS_H

class Settings
{
    double const sensitivity_;
    double const acceleration_;
    double const offset_;
    double const power_;
    double const pre_scale_x_;
    double const pre_scale_y_;
    double const post_scale_x_;
    double const post_scale_y_;
    double const angle_adjustment_;
    double const angle_snapping_;
    double const speed_cap_;
    bool const fancy_output_;
public:
    class Builder;
    Settings(
        double const sensitivity,
        double const acceleration,
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
     sensitivity_(sensitivity),
     acceleration_(acceleration),
     offset_(offset),
     power_(power),
     pre_scale_x_(pre_scale_x),
     pre_scale_y_(pre_scale_y),
     post_scale_x_(post_scale_x),
     post_scale_y_(post_scale_y),
     angle_adjustment_(angle_adjustment),
     angle_snapping_(angle_snapping),
     speed_cap_(speed_cap),
     fancy_output_(fancy_output)
    {}
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

class Settings::Builder
{
    double sensitivity_ = 0.0;
    double acceleration_ = 0.0;
    double offset_ = 0.0;
    double power_ = 0.0;
    double pre_scale_x_ = 0.0;
    double pre_scale_y_ = 0.0;
    double post_scale_x_ = 0.0;
    double post_scale_y_ = 0.0;
    double angle_adjustment_ = 0.0;
    double angle_snapping_ = 0.0;
    double speed_cap_ = 0.0;
    bool fancy_output_ = false;
public:
    Settings build() const {
        return
            Settings(
                sensitivity_,
                acceleration_,
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
