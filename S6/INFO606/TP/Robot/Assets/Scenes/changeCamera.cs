﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class changeCamera : MonoBehaviour
{

    GameObject MainCamera;
    GameObject CameraMain;
    GameObject CameraCube;
    GameObject CameraRobot;
    GameObject Robot;
    GameObject Cube;
    GameObject Main; 
    Vector3 offsetMain;
    Vector3 offsetCube;
    Vector3 offsetRobot;
    Transform target;
    float smooth = 5.0f;
    float tiltAngle = 60.0f;
    // Start is called before the first frame update
    bool anim = false;
    void Start()
    {
        MainCamera = GameObject.FindGameObjectWithTag("MainCamera");
        CameraMain = GameObject.FindGameObjectWithTag("CameraMain");
        CameraCube = GameObject.FindGameObjectWithTag("CameraCube");
        CameraRobot = GameObject.FindGameObjectWithTag("CameraRobot");
        Robot = GameObject.FindGameObjectWithTag("Robot");
        Cube = GameObject.FindGameObjectWithTag("Cube");
        Main = GameObject.FindGameObjectWithTag("Main");
        MainCamera.SetActive(true);
        CameraMain.SetActive(false);
        CameraCube.SetActive(false);
        CameraRobot.SetActive(false);
        offsetMain = CameraMain.transform.position - Main.transform.position;
        offsetCube = CameraCube.transform.position - Cube.transform.position;
        offsetRobot = CameraRobot.transform.position - Robot.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Keypad1))
        {
            MainCamera.SetActive(true);
            CameraMain.SetActive(false);
            CameraCube.SetActive(false);
            CameraRobot.SetActive(false);
        }
        if(Input.GetKeyDown(KeyCode.Keypad2)){
            MainCamera.SetActive(false);
            CameraMain.SetActive(true);
            CameraCube.SetActive(false);
            CameraRobot.SetActive(false);
        }
        if (Input.GetKeyDown(KeyCode.Keypad3))
        {
            MainCamera.SetActive(false);
            CameraMain.SetActive(false);
            CameraCube.SetActive(true);
            CameraRobot.SetActive(false);
        }
        if (Input.GetKeyDown(KeyCode.Keypad4))
        {
            MainCamera.SetActive(false);
            CameraMain.SetActive(false);
            CameraCube.SetActive(false);
            CameraRobot.SetActive(true);
        }
        if(Input.GetKeyDown(KeyCode.A)){
            if(anim){
                anim = false;
            }
            else{
                anim = true;
            }
        }

        if(anim){
        //ROTATION CAMERA MAIN
        // Smoothly tilts a transform towards a target rotation.
        float tiltAroundZ = Input.GetAxis("Horizontal") * tiltAngle;
        float tiltAroundX = Input.GetAxis("Vertical") * tiltAngle;

        // Rotate the cube by converting the angles into a quaternion.
        CameraMain.transform.rotation = Quaternion.Euler(tiltAroundX, 0, tiltAroundZ);

        // Dampen towards the target rotation
        CameraMain.transform.rotation = Quaternion.Slerp(transform.rotation, CameraMain.transform.rotation, Time.deltaTime * smooth);
        }
    }
    void LateUpdate() {
        CameraMain.transform.position = Main.transform.position + offsetMain;
        CameraRobot.transform.position = Robot.transform.position + offsetCube;
        CameraCube.transform.LookAt(Cube.transform);
    }
}
