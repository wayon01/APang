using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GaugeStar : MonoBehaviour {
    private bool isClosed;
    private UILabel label;
    private float time;

    // Use this for initialization
    void Start() {
        isClosed = false;
        label = null;
        time = 0;
    }

    // Update is called once per frame
    void Update() {

        if (!isClosed) return;

        if (label != null) {
            time += Time.deltaTime;
            label.color = Color.Lerp(Color.black, Color.white, time);
            if (label.color == Color.white) {
                label = null;
            }
        }
    }

    public void SetClosed() {
        isClosed = true;

        for (int i = 0; i < transform.childCount; i++) {
            GameObject tmp = transform.GetChild(i).gameObject;

            //라벨
            if (tmp.transform.name == "Label") {
                label = tmp.GetComponent<UILabel>();
                continue;
            }

            if (tmp.transform.name == "Grade_on") {
                tmp.GetComponent<TweenScale>().enabled = true;
                continue;
            }
        }

    }

    public void SetValue(int value) {

        for (int i = 0; i < transform.childCount; i++) {
            GameObject tmp = transform.GetChild(i).gameObject;

            //라벨
            if (tmp.transform.name == "Label") {
                tmp.GetComponent<UILabel>().text = value.ToString();
            }
        }
    }


    public bool isStarClosed() {
        return isClosed;
    }
}
