using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StageSoundBt : MonoBehaviour {

    public GameObject ButtonSpriteX;
    public bool isMute;

    private TweenRotation bt_x_rotation;
    private UIButton btButton;

	// Use this for initialization
	void Start () {
	    isMute = false;
	    bt_x_rotation = ButtonSpriteX.GetComponent<TweenRotation>();
	    btButton = GetComponent<UIButton>();

        
    }
	
	// Update is called once per frame
	void OnClick () {
	        isMute = !isMute;

            bt_x_rotation.enabled = true;
            

            if (isMute) {
	            bt_x_rotation.@from = Vector3.zero;
	            bt_x_rotation.to = new Vector3(0, 0, 45);
	        }
	        else {
                bt_x_rotation.@from = new Vector3(0, 0, 45);
                bt_x_rotation.to = Vector3.zero;
            }

        bt_x_rotation.Reset();

    }

}
