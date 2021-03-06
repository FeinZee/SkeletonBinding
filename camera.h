#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
#include <QVector3D>
#include "param.h"
// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.05f;
const float ZOOM        =  45.0f;

/*
 * 此类是摄像机类
 * 参考：https://learnopengl-cn.github.io/
 */
class Camera
{
public:
    Camera();

    // camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix()
    {
        QMatrix4x4 viewMatrix;
        viewMatrix.lookAt(Position, Position + Front, Up);
        return viewMatrix;
    }

    void lookFrom(char direction){
        if (direction == 'f') {
            Position = QVector3D(0, 0.8, 3);
            Yaw = YAW;
            Pitch = PITCH;
            Front = QVector3D(0, 0, -1);
        }
        if (direction == 't') {
            Position = QVector3D(0, 3, 0);
            Yaw = YAW;
            Pitch = -89;
            Front = QVector3D(0, -1, 0);
        }
        if (direction == 'l') {
            Position = QVector3D(-3, 0.8, 0);
            Yaw = 0;
            Pitch = 0;
            Front = QVector3D(1, 0, 0);
        }
        if (direction == 'r') {
            Position = QVector3D(3, 0.8, 0);
            Front = QVector3D(-1, 0, 0);
            Yaw = -180;
            Pitch = 0;
        }
        WorldUp = QVector3D(0.0f, 1.0f, 0.0f);

        Zoom = ZOOM;
        updateCameraVectors();
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(char direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == 'w')
            Position += Front * velocity;
        if (direction == 's')
            Position -= Front * velocity;
        if (direction == 'a')
            Position -= Right * velocity;
        if (direction == 'd')
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 89.0f)
            Zoom = 89.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        float toRadius = PI / 180;
        Front.setX(cos(Yaw * toRadius) * cos(Pitch * toRadius));
        Front.setY(sin(Pitch * toRadius));
        Front.setZ(sin(Yaw * toRadius) * cos(Pitch * toRadius));
        Front.normalize();
        // also re-calculate the Right and Up vector
        Right = QVector3D::crossProduct(Front, WorldUp);
        Right.normalize();
        Up = QVector3D::crossProduct(Right, Front);
        Up.normalize();
    }

};

#endif // CAMERA_H
