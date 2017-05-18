using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class APlayer : AActor {

    public bool isBlocked;
    public Animator animator;

    public float SpeechBubbleAnimationTime;

    private bool isInPortal;
    private Vector3 targetScale;
    private float PortalTime;

    private GameSystemMgr gameSystemMgr;
    private TileMgr tileMgr;

    private TweenScale tweenScale;
    private bool isSpeechActived;
    private bool isSpeechAnimation;
    private string SpeechText;
    private UILabel SpeechLabel;
    private float SpeechBubbleDelayTime;

    // Use this for initialization
    void Start () {
	    isBlocked = false;
	    isInPortal = false;
        targetScale = Vector3.one;
	    gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
	    tileMgr = GameObject.Find("TileManager").GetComponent<TileMgr>();
        tweenScale = GameObject.Find("SpeechBubble").GetComponent<TweenScale>();
        SpeechLabel = GameObject.Find("speech_str").GetComponent<UILabel>();
        isSpeechActived = false;
        isSpeechAnimation = false;
        SpeechText = "";
        SpeechBubbleDelayTime = 0;
    }

    public override void Init() {
        base.Init();
        
        GameObject tmp = tileMgr.GetTile(positionId);
        if (tmp == null) return;

        TileObject temp_tile = tmp.GetComponent<TileObject>();
        string str = temp_tile.GetSpeechBubble();

        if (!string.IsNullOrEmpty(str)) {
            tweenScale.enabled = true;

            tweenScale.@from = Vector3.zero;
            tweenScale.to = new Vector3(0.25f, 0.25f, 1);

            tweenScale.Reset();
            isSpeechActived = true;
            isSpeechAnimation = true;
            SpeechLabel.text = "";
            SpeechText = str;
            SpeechBubbleDelayTime = 0;
        }
    }

    // Update is called once per frame
	void Update () {
        PortalTime += Time.deltaTime;
        if (transform.localScale != targetScale) {

	        transform.localScale = Vector3.Lerp(transform.localScale, targetScale, PortalTime);
	    }

	    if (isSpeechActived && isSpeechAnimation) {
            SpeechBubbleDelayTime += Time.deltaTime;
            int index = SpeechLabel.text.Length;

	        if (index < SpeechText.Length && SpeechBubbleDelayTime > (float)SpeechBubbleAnimationTime * index) {
	            SpeechLabel.text += SpeechText[index];
	        }else if (index >= SpeechText.Length) {
	            SpeechBubbleDelayTime = 0;
	            isSpeechAnimation = false;

	        }
	    }

	    if (gameSystemMgr.isPlayerMovingUp) {
	        TileObject tmp = tileMgr.GetTile(positionId).GetComponent<TileObject>();
	        string str = tmp.GetSpeechBubble();

	        if (!string.IsNullOrEmpty(str)) {
	            tweenScale.enabled = true;

	            tweenScale.@from = Vector3.zero;
	            tweenScale.to = new Vector3(0.25f, 0.25f, 1);

	            tweenScale.Reset();
	            isSpeechActived = true;
                isSpeechAnimation = true;
                SpeechText = str;
	            SpeechLabel.text = "";
                SpeechBubbleDelayTime = 0;
            }
	        else if(isSpeechActived) {
	            isSpeechActived = false;
                isSpeechAnimation = false;
                tweenScale.enabled = true;

                tweenScale.@from = new Vector3(0.25f, 0.25f, 1);
                tweenScale.to = Vector3.zero;

                tweenScale.Reset();
                SpeechText = "";
                SpeechBubbleDelayTime = 0;

            }
	    }
        

    }

    void LateUpdate() {
        Quaternion q = Camera.main.transform.rotation;
        if(!Input.GetMouseButtonDown(0) && !Input.GetMouseButton(0))
            transform.rotation = Quaternion.Lerp(transform.rotation, Camera.main.transform.rotation, 0.1f);
    }

    public override void Die() {
        
    }

    public void OnTakePortal() {

        isInPortal = !isInPortal;

       
        animator.SetBool("isDisappear", isInPortal);
        
        PortalTime = 0;

        if (isInPortal) {
            targetScale = Vector3.zero;
            PortalTime = -0.5f;
        }
        else {
            targetScale = Vector3.one;
        }

    }
   

}
