using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GaugeMoveCount : MonoBehaviour {

    public int count;
    private bool isMove;

    private UILabel label;
    private TweenRotation tweenRotation;

    private float time;

	// Use this for initialization
	void Start () {
	    count = 0;
	    time = 0;
	    isMove = false;
	    label = GetComponent<UILabel>();
	    tweenRotation = GetComponent<TweenRotation>();
	}
	
	// Update is called once per frame
	void Update () {
	    if (isMove && !tweenRotation.enabled) {
	        isMove = false;
            label.text = count.ToString();
            SetTweenRotation(new Vector3(0, 90, 0), Vector3.zero);
	    }
	}

    public void SetCountValue(int n) {
        count = n;
        isMove = true;
        SetTweenRotation(Vector3.zero, new Vector3(0, 90, 0));
    }

    private void SetTweenRotation(Vector3 from, Vector3 to) {
        tweenRotation.enabled = true;

        tweenRotation.@from = from;
        tweenRotation.to = to;

        tweenRotation.Reset();
    }
}
