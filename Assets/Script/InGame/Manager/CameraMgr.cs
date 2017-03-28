using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour {

    public Camera MainCamera = null;
    public Vector2 Rotation;
    public float FinalRotationY = 0f;
    public float Sensitivity = 2f;

    public GameObject GameSystemManager;

    public bool isRelax = true;

    private Quaternion quaternion;
    private Vector2 mouse_delta;

    private GameSystemMgr gameSystemMgr;

    // Use this for initialization
    void Start() {
        quaternion = Quaternion.identity;
        Rotation = new Vector2(0, 0);
        mouse_delta = new Vector2(0, 0);
        gameSystemMgr = GameSystemManager.GetComponent<GameSystemMgr>();
    }

    // Update is called once per frame
    void Update() {

        if (gameSystemMgr.isPlayerMoving) {
            return;
        }

        if (Input.GetMouseButtonDown(0)) {
            Rotation.Set(0, 0);
            mouse_delta.Set(-Input.mousePosition.y / Sensitivity, Input.mousePosition.x / Sensitivity);
        }

        if (Input.GetMouseButton(0)) {

            Rotation.x += -Input.mousePosition.y / Sensitivity - mouse_delta.x;
            Rotation.y += Input.mousePosition.x / Sensitivity - mouse_delta.y;
            mouse_delta.Set(-Input.mousePosition.y / Sensitivity, Input.mousePosition.x / Sensitivity);

            if (Rotation.x < 0) {
                Rotation.x = 0;
            }else if (Rotation.x > 90) {
                Rotation.x = 90;
            }

            if (Math.Abs(Rotation.y) < 15f && isRelax) {

                if (Math.Abs(Rotation.x) > 15f) {
                    isRelax = false;

                    quaternion.eulerAngles = new Vector3(Rotation.x, FinalRotationY, 0);
                    transform.rotation = Quaternion.Lerp(transform.rotation, quaternion, Time.deltaTime * 10);
                }
                else {
                    isRelax = true;
                }

                return;
            }
            isRelax = false;

            quaternion.eulerAngles = new Vector3(Rotation.x, FinalRotationY + Rotation.y, 0);
            transform.rotation = Quaternion.Lerp(transform.rotation, quaternion, Time.deltaTime * 10);

        }else if (Input.GetMouseButtonUp(0)) {

            if (Math.Abs(Rotation.y) < 15f)
                return;
                // ReSharper disable once PossibleLossOfFraction
                FinalRotationY = (int)(FinalRotationY + Rotation.y + (FinalRotationY + Rotation.y > 0 ? 45 : -45))/90*90f;

            if (Math.Abs(FinalRotationY) == 360) {
                FinalRotationY = 0;
            }else if (Math.Abs(FinalRotationY) == 270) {
                FinalRotationY = (FinalRotationY > 0) ? -90 : 90;
            }else if (FinalRotationY == -180) {
                FinalRotationY = 180;
            }
        }
        else {
            quaternion.eulerAngles = new Vector3(0, FinalRotationY, 0);
            transform.rotation = Quaternion.Lerp(transform.rotation, quaternion, Time.deltaTime * 15);

            if (!isRelax) {
                isRelax = Math.Abs(transform.eulerAngles.x) < 0.0001f && Math.Abs(transform.eulerAngles.z) < 0.0001f;
            }
        }
    }

}